#include "ultrasonicSensorTLM.cpp"
#include "ultrasonicSensor.cpp"
#include "calc_dist_TLM.cpp"
#include "camara_sensor_TLM.cpp"
#include "equalizerTLM.cpp"
#include "InterpolationTLM.cpp"

#include "utils.hpp"

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

    cout << "LAST TARGET MODULE" << endl << endl;
    cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;
    
    unsigned char* filtered_image = trans_pending->get_data_ptr();

    // Save output image
    stbi_write_jpg("filtered.jpg", COLS/2, ROWS/2, 1, filtered_image, (ROWS/2)*(COLS/2));

    free(filtered_image);

    // Call on backward path to complete the transaction
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
  UltrasonicSensorTLM*  uSensor;
  CalcDistTLM*          calcDist;
  CamaraSensTLM*        cSens;
  EqualizerTLM*         compressor;
  //InterpolationTLM*     interpolation;
  DefaultTarget*        defaultTarget;

  SC_CTOR(Top)
  {
    // Instantiate components
    uSensor       = new UltrasonicSensorTLM("UltrasonicSensorTLM");
    calcDist      = new CalcDistTLM("CalcDistTLM");
    cSens         = new CamaraSensTLM("CamaraSensTLM");
    compressor    = new EqualizerTLM("equalizerTLM");
    //interpolation = new InterpolationTLM("InterpolationTlM");
    defaultTarget = new DefaultTarget("defaultTarget");

    // Bind sockets
    uSensor->initiator_socket.bind(calcDist->target_socket);
    calcDist->initiator_socket.bind(cSens->target_socket);
    cSens->initiator_socket.bind(compressor->target_socket);
    //compressor->initiator_socket.bind(interpolation->target_socket);
    compressor->initiator_socket.bind(defaultTarget->target_socket);
    //interpolation->initiator_socket.bind(defaultTarget->target_socket);
  }
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  sc_start();
  return 0;
}