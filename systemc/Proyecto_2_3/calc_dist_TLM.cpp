#include "utils.hpp"

#include "memory_map.h"
#include "RegisterBank.hpp"
#include "RegisterBank.cpp"
#include "global_register_bank.hpp"

struct CalcDistTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<CalcDistTLM> target_socket;

  // TLM2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<CalcDistTLM> initiator_socket;

  // Internal data buffer used by initiator with generic payload   
  int data;

  SC_CTOR(CalcDistTLM)   
  : target_socket("CalcDistTLM:target"),
  initiator_socket("CalcDistTLM:initiator")  // Construct and name socket   
  
  {   
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &CalcDistTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &CalcDistTLM::initiator_nb_transport_bw);
    
    calcDistMod = new dist_calc("cdM")

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
        double time_output = 0.0;
        double echo_signal = 0.0;

        unsigned char* data_ptr = trans_pending->get_data_ptr();
        memcpy(&time_output, data_ptr, sizeof(double));
        memcpy(&echo_signal, data_ptr + sizeof(double), sizeof(double));

        cout << "From TARGET" << endl;
        cout << "Delay time coming from sensor : " << time_output << endl;
        cout << "Echo signal coming from sensor: " << echo_signal << endl;

        cout << "FROM REGISTER STATUS OF SENS_ACTIVE: " << global_register_bank.read_bits(REG_BASE_1+0x2,0x1) << endl;

        tlm::tlm_phase phase = tlm::BEGIN_RESP;
        status = target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);

        // Check value returned from nb_transport   
        if (status != tlm::TLM_ACCEPTED) {
        cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        }

        // Process DATA coming from Ultrasonic Sensor
        // In general, we don't need the distances, we need to activate the camara

        int Camara_active = 0;

        if (global_register_bank.read_bits(REG_BASE_1+0x2,0x1) == 0x1){
            Camara_active = 1;
            //Since reading was detected, now disabling it for next iteration
            global_register_bank.write_bits(REG_BASE_1+0x2,0x1,0x0);
        }

        // TLM2 generic payload transaction
        tlm::tlm_generic_payload trans;
        ID_extension* id_extension = new ID_extension;
        trans.set_extension( id_extension ); // Add the extension to the transaction

        id_extension->transaction_id = generateUniqueID();

        tlm::tlm_phase phase = tlm::BEGIN_REQ;   
        sc_time delay = sc_time(10, SC_NS);   

        //Data to be sent        
        // These lines should be modified to include the important data coming out of the calc dist module 
        trans.set_data_ptr( Camara_active );   
        trans.set_data_length( sizeof(Camara_active) );

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