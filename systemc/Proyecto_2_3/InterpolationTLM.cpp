#include "utils.hpp"

struct InterpolationTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<InterpolationTLM> target_socket;

  // TLM-2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<InterpolationTLM> initiator_socket;

  sc_event  e1;
  tlm::tlm_generic_payload* trans_pending;   
  tlm::tlm_phase phase_pending; 
  sc_time delay_pending;
  interpolation* inter;

  // Construct and name socket   
  SC_CTOR(InterpolationTLM) 
  : target_socket("InterpolationTLM:target")
  , initiator_socket("InterpolationTLM:initiator")
  {
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &InterpolationTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &InterpolationTLM::initiator_nb_transport_bw);

    inter = new interpolation("it");

    SC_THREAD(thread_process);
  }

  void thread_process()  
{
  tlm::tlm_sync_enum status;

  while(true) {

    // Wait for an event to pop out of the back end of the queue   
    wait(e1);

    ID_extension* id_extension = new ID_extension;
    trans_pending->get_extension(id_extension); 

    // Obliged to set response status to indicate successful completion   
    trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);  

    cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;

    unsigned char* ptr = trans_pending->get_data_ptr();
    uint8_t** image = (uint8_t**)ptr;

    // Call on backward path to complete the transaction
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    status = target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);

    // Check value returned from nb_transport   
    if (status != tlm::TLM_ACCEPTED) {
      cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
    }

    // Save output image
    uint8_t equalized_image[ROWS][COLS] = { 0 };
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
        equalized_image[i][j] = image[i][j];
        }
    }
    //
    //
    //
    // Process image here
    int width = 434; // Example width, adjust according to your image
    int height = 323; // Example height, adjust according to your image
    int channels = 1; // RGBA format assumed


    // Instantiate interpolation module
    
    unsigned char* filtered_image = (unsigned char*)malloc(width/2 * height/2 * channels);

    inter->interpolate_image((unsigned char*)equalized_image, width, height, filtered_image, width/2, height/2, channels);


    freeMatrix(image, ROWS);

    //
    //
    //

    tlm::tlm_generic_payload trans;
    id_extension = new ID_extension;
    trans.set_extension(id_extension);
    
    id_extension->transaction_id = generateUniqueID();

    phase = tlm::BEGIN_REQ;   
    sc_time delay = sc_time(10, SC_NS);   

    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);   
    trans.set_data_ptr( reinterpret_cast<unsigned char*>(filtered_image) );   
    trans.set_data_length( sizeof(image) );   

    cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
    status = initiator_socket->nb_transport_fw(trans, phase, delay);  // Non-blocking transport call   
  
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