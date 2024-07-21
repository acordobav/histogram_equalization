#include "utils.hpp"

#include "memory_map.h"
//#include "RegisterBank.hpp"
//#include "RegisterBank.cpp"
//#include "global_register_bank.hpp"

struct CalcDistTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<CalcDistTLM> target_socket;

  // TLM2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<CalcDistTLM> initiator_socket;

  // Internal data buffer used by initiator with generic payload
  sc_event  e1;
  tlm::tlm_generic_payload* trans_pending;   
  tlm::tlm_phase phase_pending; 
  sc_time delay_pending;  
  dist_calc* calcDistMod;

  //Internal variables
  int data;

  //To fully connect the Dist Calc module
  //sca_tdf::sca_signal<bool> sens_active;
  sc_signal<bool> sens_active;
  sc_signal<double> dist_cm; 
  sc_signal<double > calc_voltage;
  sc_signal<double> echo_signal;
  sc_signal<double> time_signal;
  sc_signal<const char*> sens_range;
  sc_signal<bool > trigger;
  sc_signal<sc_int <32> > count_near;
  sc_signal<sc_int <32> > count_half;
  sc_signal<sc_int <32> > count_far;

  //To be able to process data with the Dist Calc module
  double time_output;
  double echo_signal_result;
  double dist_result;
  bool sens_active_result;
  int sens_active_transmit;

  SC_CTOR(CalcDistTLM)   
  : target_socket("CalcDistTLM:target"),
  initiator_socket("CalcDistTLM:initiator")  // Construct and name socket   
  
  {   
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &CalcDistTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &CalcDistTLM::initiator_nb_transport_bw);
    
    calcDistMod = new dist_calc("cdM");
    calcDistMod->dist_cm(dist_cm);
    calcDistMod->calc_voltage(calc_voltage);
    calcDistMod->sens_range(sens_range);
    calcDistMod->trigger(trigger);
    calcDistMod->echo_signal(echo_signal);
    calcDistMod->time_signal(time_signal);
    calcDistMod->count_near(count_near);
    calcDistMod->count_half(count_half);
    calcDistMod->count_far(count_far);
    calcDistMod->sens_active(sens_active);

    SC_THREAD(thread_process);   
  }   

  void thread_process()   
  {
    tlm::tlm_sync_enum status;

    while(true){
        // Wait for an event to pop out of the back end of the queue   
        wait(e1);

        ID_extension* id_extension = new ID_extension;
        trans_pending->get_extension(id_extension); 

        // Obliged to set response status to indicate successful completion   
        trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);  

        cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;
        
        //Variables to store the incoming data

        if (global_register_bank.read_bits(REG_BASE_1+0x2,0x1)){
            unsigned char* data_ptr = trans_pending->get_data_ptr();
            memcpy(&time_output, data_ptr, sizeof(double));
            memcpy(&echo_signal_result, data_ptr + sizeof(double), sizeof(double));

            cout << endl;
            cout << "From CALC DIST TARGET" << endl;
            cout << "Delay time coming from sensor : " << time_output << endl;
            cout << "Echo signal coming from sensor: " << echo_signal_result << endl;

            //cout << "FROM REGISTER STATUS OF SENS_ACTIVE: " << global_register_bank.read_bits(REG_BASE_1+0x2,0x1) << endl;

            //Since reading was detected, now disabling it for next iteration
            global_register_bank.write_bits(REG_BASE_1+0x2,0x1,0x0);
        }
        sens_active_result = false;
        sens_active_transmit = 0;

        // CALC DIST Module is going to provide the required data
        //cout << "Simulation of the Calc Dist module" << endl;

        calcDistMod->delay_start(); 
        echo_signal.write(echo_signal_result); // echo enabled
        time_signal.write(time_output);
        //cout << "Distance: " << dist_cm.read() << endl;
        dist_result = calcDistMod->read_distance();
        sens_active_result = calcDistMod->read_sens_active();
        calcDistMod->e_wait();
        //cout << "Distance: " << dist_cm.read() << endl;

        //dist_result = calcDistMod->read_sens_active();

        //cout << "From Dist Calc Module/Init" << endl;
        ///cout << "Distance: " << dist_result << endl;
        //cout << "Sens Active value: " << sens_active_result << endl;

        if (sens_active_result){
            //Register bit to indicate that an animal is in front of the camara
            //Capture bit being set to TRUE
            global_register_bank.write_bits(REG_BASE_2+0x2,0x1,0x1);
            sens_active_transmit = 1;
        }
        sens_active_transmit = 1; //Just for debugging

        tlm::tlm_phase phase = tlm::BEGIN_RESP;
        //cout << name() << "From TEST1" << endl;
        status = target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);
//cout << name() << "From TEST2" << endl;
        // Check value returned from nb_transport   
        if (status != tlm::TLM_ACCEPTED) {
        cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        }

        // Process DATA coming from Ultrasonic Sensor
        // In general, we don't need the distances, we need to activate the camara

        // TLM2 generic payload transaction
        tlm::tlm_generic_payload trans;
        id_extension = new ID_extension;
        trans.set_extension( id_extension ); // Add the extension to the transaction

        id_extension->transaction_id = generateUniqueID();

        phase = tlm::BEGIN_REQ;   
        sc_time delay = sc_time(10, SC_NS);   

        //Data to be sent        
        // These lines should be modified to include the important data coming out of the calc dist module   
        trans.set_data_length( sizeof(sens_active_transmit) );
        unsigned char data[sizeof(int)]; //Here it is going to be concatenated the data to be sent
        memcpy(data, &sens_active_transmit, sizeof(int));
        trans.set_data_ptr( data ); 

        cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        status = initiator_socket->nb_transport_fw( trans, phase, delay );  // Non-blocking transport call   
    
        // Check value returned from nb_transport   
        if (status != tlm::TLM_ACCEPTED) {
            cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        }
    }   
  }


  // TLM-2 non-blocking transport method
  virtual tlm::tlm_sync_enum target_nb_transport_fw(
    tlm::tlm_generic_payload& trans,
    tlm::tlm_phase& phase,
    sc_time& delay)
    {
    //cout << "Será que no lo escribe8....     " << endl;
        ID_extension* id_extension = new ID_extension;
        trans.get_extension(id_extension);

        if (phase != tlm::BEGIN_REQ) {
        cout << name() << " unknown phase " << phase << endl;
        return tlm::TLM_ACCEPTED;
        }

        cout << name() << " BEGIN_REQ RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;      

        // Now queue the transaction until the annotated time has elapsed
        trans_pending=&trans;
        phase_pending=phase;
        delay_pending=delay;

        e1.notify();

        //Delay
        wait(delay);

        return tlm::TLM_ACCEPTED;
    }


  // ************************************************
  // TLM2 backward path non-blocking transport method   
  // ************************************************
  virtual tlm::tlm_sync_enum initiator_nb_transport_bw(
    tlm::tlm_generic_payload& trans,   
    tlm::tlm_phase&           phase,
    sc_time&                  delay)
  {   
  //cout << "Será que no lo escribe7....     " <<endl;
    ID_extension* id_extension = new ID_extension;
    trans.get_extension( id_extension ); 
    
    if (phase != tlm::BEGIN_RESP) {
      cout << name() << " unknown phase " << phase << endl;
      return tlm::TLM_ACCEPTED;
    }

    // Initiator obliged to check response status   
    if (trans.is_response_error() )   
      SC_REPORT_ERROR("TLM2", "Response error from nb_transport");   
      //   cout << "Será que no lo escribe10....     " <<endl; 
    //Delay
    wait(delay);
    
    cout << name () << " BEGIN_RESP RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
    
    return tlm::TLM_ACCEPTED;   
  }
};
