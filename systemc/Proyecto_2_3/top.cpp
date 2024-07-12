#include "equalizerTLM.cpp"

struct DefaultInitiator: sc_module
{
  // TLM2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<DefaultInitiator> initiator_socket;

  // Internal data buffer used by initiator with generic payload   
  int data;   

  SC_CTOR(DefaultInitiator)   
  : initiator_socket("DefaultInitiator:initiator")  // Construct and name socket   
  {   
    // Register callbacks for incoming interface method calls
    initiator_socket.register_nb_transport_bw(this, &DefaultInitiator::nb_transport_bw);
    
    SC_THREAD(thread_process);   
  }

  // ************************************************
  // TLM2 backward path non-blocking transport method   
  // ************************************************
  virtual tlm::tlm_sync_enum nb_transport_bw(
    tlm::tlm_generic_payload& trans,   
    tlm::tlm_phase&           phase,
    sc_time&                  delay)
  {   
    tlm::tlm_command cmd = trans.get_command();   
    sc_dt::uint64    adr = trans.get_address();   
    
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

  void thread_process()   
  {
    uint8_t** image = createMatrix(ROWS, COLS);

    // Load image
    int width, height, channels;
    uint8_t* img = stbi_load("../Proyecto_1/Module_Equalization/sydney.jpg", &width, &height, &channels, 0);

    // Extract the red channel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            image[y][x] = img[index]; // Red channel is the first byte in each pixel
        }
    }

    // Free the image memory
    stbi_image_free(img);

    // TLM2 generic payload transaction
    tlm::tlm_generic_payload trans;
    ID_extension* id_extension = new ID_extension;
    trans.set_extension( id_extension ); // Add the extension to the transaction

    // Generate a random sequence of reads and writes   
    for (int i = 15; i < 16; i++)   
    {  
      id_extension->transaction_id = generateUniqueID();

    // int i = 15; 
      tlm::tlm_phase phase = tlm::BEGIN_REQ;   
      sc_time delay = sc_time(10, SC_NS);   
            
      tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);   
      trans.set_data_ptr( reinterpret_cast<unsigned char*>(image) );   
      trans.set_data_length( sizeof(image) );   

      // Other fields default: byte enable = 0, streaming width = 0, DMI_hint = false, no extensions   

      //Delay for BEGIN_REQ
      wait( sc_time(10, SC_NS) );
      tlm::tlm_sync_enum status;   
      
      cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      status = initiator_socket->nb_transport_fw( trans, phase, delay );  // Non-blocking transport call   
   
      // Check value returned from nb_transport   
      if (status != tlm::TLM_ACCEPTED) {
        cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      }
      
    }   
  }   

};

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
    
    unsigned char* ptr = trans_pending->get_data_ptr();
    uint8_t** filtered_image = (uint8_t**)ptr;

    // Call on backward path to complete the transaction
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);

    // Save output image
    uint8_t save_image[ROWS][COLS] = { 0 };
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        save_image[i][j] = filtered_image[i][j];
      }
    }
    stbi_write_jpg("filtered.jpg", COLS, ROWS, 1, save_image, ROWS*COLS);

    freeMatrix(filtered_image, ROWS);
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
  DefaultInitiator*  initiator;
  EqualizerTLM* compressor;
  DefaultTarget* defaultTarget;

  SC_CTOR(Top)
  {
    // Instantiate components
    initiator     = new DefaultInitiator("initiator");
    compressor    = new EqualizerTLM("equalizerTLM");
    defaultTarget = new DefaultTarget("defaultTarget");

    // Bind sockets
    initiator->initiator_socket.bind(compressor->target_socket);
    compressor->initiator_socket.bind(defaultTarget->target_socket);
  }
};

int sc_main(int argc, char* argv[])
{
  Top top("top");
  sc_start();
  return 0;
}

