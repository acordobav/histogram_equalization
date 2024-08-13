#include "utils.hpp"

struct InterpolationTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<InterpolationTLM> target_socket;

  // TLM-2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<InterpolationTLM> initiator_socket;
  tlm_utils::simple_initiator_socket<EqualizerTLM> register_socket;

  sc_event  e1;
  tlm::tlm_generic_payload* trans_pending;   
  tlm::tlm_phase phase_pending; 
  sc_time delay_pending;
  interpolation* inter;

  // Construct and name socket   
  SC_CTOR(InterpolationTLM) 
  : target_socket("InterpolationTLM:target")
  , initiator_socket("InterpolationTLM:initiator")
  , register_socket("InterpolationTLM:register")
  {
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &InterpolationTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &InterpolationTLM::initiator_nb_transport_bw);

    inter = new interpolation("it");

    SC_THREAD(thread_process);
  }

  uint32_t register_data_get(uint64_t address) {
    uint32_t data = 0;
    
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    trans.set_command(tlm::TLM_READ_COMMAND);
    trans.set_data_ptr((uint8_t*)&data);
    trans.set_data_length(sizeof(uint32_t));
    trans.set_address(address);

    register_socket->b_transport(trans, delay);

    return data;  
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

    // Valores principales
    int original_width = register_data_get(REG_IMAGE_COLS); // Asumiendo que COLS es el ancho de la imagen
    int original_height = register_data_get(REG_IMAGE_ROWS); // Asumiendo que ROWS es la altura de la imagen
    int channels = 1; // Asumiendo una imagen en escala de grises o un canal
    int new_width = newCOLS;
    int new_height = newROWS;

    uint8_t** interpol_image = createMatrix(new_height,new_width);

    // Interpolar la imagen
    inter->interpolate_image((uint8_t**)image, original_width, original_height, interpol_image, new_width, new_height);

    sc_time process_delay = sc_time(100, SC_US);   
    wait(process_delay);


// [INFO] Para ver las imagenes:

// Convertir imagen de SALIDA interpolada a un vector
    unsigned char* interpolated_image = (unsigned char*)malloc(new_width * new_height * channels);
    if (interpolated_image == NULL) {
        cerr << "Error: no se pudo asignar memoria para interpolated_image" << endl;
        exit(1);
    }

    for (int i = 0; i < new_height; i++) {
        memcpy(interpolated_image + (i * new_width * channels), interpol_image[i], sizeof(uint8_t) * new_width * channels);
    }

    // Guardar la imagen de entrada interpolada
    if (!stbi_write_jpg("2_interpolated.jpg", new_width, new_height, channels, interpolated_image, 100)) {
        cerr << "Error: no se pudo guardar la imagen" << endl;
    }
    free(interpolated_image);
// Convertir imagen de ENTRADA  a un vector
    unsigned char* test_image = (unsigned char*)malloc(original_width * original_height * channels);
    if (test_image == NULL) {
        cerr << "Error: no se pudo asignar memoria para interpolated_image" << endl;
        exit(1);
    }

    for (int i = 0; i < original_height; i++) {
        memcpy(test_image + (i * original_width * channels), image[i], sizeof(uint8_t) * original_width * channels);
    }

    // Guardar la imagen 
    if (!stbi_write_jpg("1_entrada.jpg", original_width, original_height, channels, test_image, 100)) {
        cerr << "Error: no se pudo guardar la imagen" << endl;
    }
    free(test_image);

//-----------------------------------------
    
    // Liberar memoria del vector de imagen
    freeMatrix(image, ROWS);


    tlm::tlm_generic_payload trans;
    id_extension = new ID_extension;
    trans.set_extension(id_extension);
    
    id_extension->transaction_id = generateUniqueID();

    phase = tlm::BEGIN_REQ;   
    sc_time delay = sc_time(10, SC_NS);   

    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);   
    trans.set_data_ptr( reinterpret_cast<unsigned char*>(interpol_image)  );   
    trans.set_data_length( sizeof(interpol_image) );   
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