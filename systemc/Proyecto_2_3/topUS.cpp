#include "ultrasonicSensorTLM.cpp"
#include "ultrasonicSensor.cpp"

//To include operations related to the Bank of Registers
#include "memory_map.h"
#include "RegisterBank.hpp"
#include "RegisterBank.cpp"
#include "global_register_bank.hpp"

//Need to remove it in the future
RegisterBank global_register_bank(0x10000, 0x10072);

struct DefaultTarget: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<DefaultTarget> target_socket;

  sc_event  e1;
  tlm::tlm_generic_payload* trans_pending;   
  tlm::tlm_phase phase_pending; 
  sc_time delay_pending;


  // Construct and name socket   
  SC_CTOR(DefaultTarget)
  : target_socket("DefaultTarget:target")
  {
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &DefaultTarget::nb_transport_fw);

    SC_THREAD(thread_process);
  }

  void thread_process()  
{
  while(true) {

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
    target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);
  }
}

  // TLM-2 non-blocking transport method
  virtual tlm::tlm_sync_enum nb_transport_fw(
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
};


SC_MODULE(Top)
{
  UltrasonicSensorTLM* uSensor;
  DefaultTarget* defaultTarget;

  SC_CTOR(Top)
  {
    // Instantiate components
    uSensor    = new UltrasonicSensorTLM("UltrasonicSensorTLM");
    defaultTarget = new DefaultTarget("defaultTarget");

    // Bind sockets
    uSensor->initiator_socket.bind(defaultTarget->target_socket);
  }
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  sc_start();
  return 0;
}