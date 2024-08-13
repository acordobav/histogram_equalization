#include "utils.hpp"
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdlib> //for random values purposes

#include "systemc.h"
#include "systemc-ams.h"

#include "memory_map.h"
#include "ultrasonicSensor.hpp"

class signal_driver : public sca_tdf::sca_module {
  public:
      sca_tdf::sca_out<double> out;

    signal_driver(sc_core::sc_module_name nm) : out("out"), signal_value(0.0) {}

    void set_attributes() {
        set_timestep(1.0, SC_MS); // Set the time step to 1 ms
    }

    void processing() {
        out.write(signal_value);
    }

    void set_signal_value(double value) {
        signal_value = value;
    }

  private:
      double signal_value;
};

/*
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
*/

struct UltrasonicSensorTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<UltrasonicSensorTLM> initiator_socket;
  tlm_utils::simple_initiator_socket<UltrasonicSensorTLM> register_socket;

  //Internal Data
  ultrasonic_sensor* us1;
  signal_driver* sig_drv;
  sca_tdf::sca_signal<double> tx_signal, out;
  sca_tdf::sca_signal<double> echo_signal;
  sca_tdf::sca_signal<double> time_output;
  int num_samples;

  // Construct and name socket   
  SC_CTOR(UltrasonicSensorTLM) 
  : initiator_socket("UltrasonicSensorTLM:initiator")
  , register_socket("UltrasonicSensorTLM:register")
  {
    // Register callbacks for incoming interface method calls
    initiator_socket.register_nb_transport_bw(this, &UltrasonicSensorTLM::initiator_nb_transport_bw);    

    us1 = new ultrasonic_sensor("us1", 40.0, 10.0, 100.0, 25000.0);
    sig_drv = new signal_driver("sig_drv");

    us1->tx_signal(tx_signal);
    sig_drv->out(tx_signal); //Signals are going to be 'connected', to produce the required behavior

    us1->echo_out(echo_signal);
    us1->time_delay(time_output);

    SC_THREAD(thread_process);
  }

  //----------------------------------------------------------------------------------------------------------------
  //----------------------------------------------------------------------------------------------------------------

  void register_data_set(uint64_t address, uint32_t value)
  {
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr((uint8_t *)&value);
    trans.set_data_length(sizeof(uint32_t));
    trans.set_address(address);

    register_socket->b_transport(trans, delay);
  }

  void thread_process()  
  {
    tlm::tlm_sync_enum status;

    // TLM2 generic payload transaction
    tlm::tlm_generic_payload trans;
    ID_extension* id_extension = new ID_extension;
    //trans.set_extension( id_extension ); // Add the extension to the transaction
    trans.set_extension( id_extension ); // Add the extension to the transaction
     
    // Generate a random sequence of reads and writes

    num_samples = 15;
    for (int i = 0; i < num_samples; ++i) 
    {
        // Simulate a pulse transmission
        sig_drv->set_signal_value(0.0);
        wait(30.0, SC_MS);

        sig_drv->set_signal_value(1.0);
        wait(30.0, SC_MS); //To match suggested wait time of the actual sensor

        us1->read_outputs();

        //Important data to be sent to the Target Socket
        double time_output_val = us1->read_detection_elapsed_time(); //elapsed time
        
        double echo_signal_val = us1->read_last_echo(); //eco signal time
        
        //Just to verify the communication is okay
        cout << endl;
        cout << "FROM ULTRASONIC SENSOR" << endl;
        cout << "Delay time being sent: " << time_output_val << endl;
        cout << "Echo signal being sent: " << echo_signal_val << endl;
        cout << endl;
        
        //Prepare to write register
        if (echo_signal_val != 0.0){
            //cout << "Writting REGISTER" << endl;
            register_data_set(REG_IS_ACTIVE0, 0x1);
        }

        //Generating unique id and fields required for the communication
        id_extension->transaction_id = generateUniqueID();
        tlm::tlm_phase phase = tlm::BEGIN_REQ;   
        sc_time delay = sc_time(10, SC_NS);   

        //Data to be sent
        trans.set_address(0);
        trans.set_data_length(2 * sizeof(double));
        unsigned char data[2 * sizeof(double)]; //Here it is going to be concatenated the data to be sent
        memcpy(data, &time_output_val, sizeof(double)); // Copy 4 bytes of time_output_val
        memcpy(data + sizeof(double), &echo_signal_val, sizeof(double)); // Copy 4 bytes of echo_signal_val
        trans.set_data_ptr(data); 

        tlm::tlm_sync_enum status;

        cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        status = initiator_socket->nb_transport_fw( trans, phase, delay );  // Non-blocking transport call   
   
        // Check value returned from nb_transport   
        if (status != tlm::TLM_ACCEPTED) 
        {
          cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        }
    }
    return;
  }

  // ************************************************
  // TLM2 backward path non-blocking transport method   
  // ************************************************
  virtual tlm::tlm_sync_enum initiator_nb_transport_bw(
    tlm::tlm_generic_payload& trans,   
    tlm::tlm_phase&           phase,
    sc_time&                  delay)
  {   
    ID_extension* id_extension = new ID_extension;
    trans.get_extension( id_extension ); 
    
    if (phase != tlm::BEGIN_RESP) {
      cout << name() << " unknown phase " << phase << endl;
      return tlm::TLM_ACCEPTED;
    }

    // Initiator obliged to check response status   
    if (trans.is_response_error() )   
      SC_REPORT_ERROR("TLM2", "Response error from nb_transport");   
          
    //Delay
    wait(delay);
    
    cout << name () << " BEGIN_RESP RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
    
    return tlm::TLM_ACCEPTED;   
  }
};
