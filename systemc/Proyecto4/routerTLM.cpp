#include "utils.hpp"
#include <queue>

struct transaction_pending {
  tlm::tlm_generic_payload* trans_pending;   
  tlm::tlm_phase phase_pending; 
  sc_time delay_pending;
};

template <unsigned int N_TARGETS>
struct RouterTLM : sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<RouterTLM>* target_socket[N_TARGETS];

  // *********************************************
  // Use tagged sockets to be able to distinguish incoming backward path calls
  // *********************************************

  tlm_utils::simple_initiator_socket_tagged<RouterTLM> initiator_socket;

  // Transaction queue
  std::queue<struct transaction_pending> trans_queue;

  sc_semaphore cv_semaphore;

  SC_CTOR(RouterTLM)
      : initiator_socket("RouterTLM:initiator")
      , cv_semaphore(0) // Initialize the semaphore with an initial count of 0
  {
    // Register callbacks for incoming interface method calls
    for (unsigned int i = 0; i < N_TARGETS; i++)
    {
      char txt[30];
      sprintf(txt, "InterpolationTLM:target%d", i);
      target_socket[i] = new tlm_utils::simple_target_socket<RouterTLM>(txt);
      target_socket[i]->register_nb_transport_fw(this, &RouterTLM::target_nb_transport_fw);
    }
    
    SC_THREAD(thread_process);
  }

  void thread_process()  
  {
    struct transaction_pending trans_queued;
    tlm::tlm_sync_enum status;
    tlm::tlm_generic_payload* trans_pending;
    tlm::tlm_phase phase_pending;
    sc_time delay_pending;

      while(true) {
    // Wait for an event to pop out of the back end of the queue   
      cv_semaphore.wait();

      // Get one transaction from the queue
      trans_queued = trans_queue.front();
      trans_queue.pop();

      trans_pending = trans_queued.trans_pending;
      phase_pending = trans_queued.phase_pending;
      delay_pending = trans_queued.delay_pending;

      ID_extension* id_extension = new ID_extension;
      trans_pending->get_extension(id_extension); 

      // Obliged to set response status to indicate successful completion   
      trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);
      

      unsigned char* ptr = trans_pending->get_data_ptr();
      uint8_t** image = (uint8_t**)ptr; // Interpreta el puntero como una matriz

    // Enviar cada elemento de la matriz a la memoria
    for (unsigned int row = 0; row < newROWS; ++row) {
      for (unsigned int col = 0; col < newCOLS; ++col) {
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        ID_extension* id_extension_new = new ID_extension;
        
        trans->set_extension(id_extension_new);
        id_extension_new->transaction_id = generateUniqueID();
        
        // Configurar la transacción
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_ptr(&image[row][col]);
        trans->set_data_length(1); // Enviar un byte a la vez
        trans->set_address(row * newCOLS + col); // Dirección basada en la posición en la matriz
        
        sc_time delay = sc_time(10, SC_NS);
        cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension_new->transaction_id << " at time " << sc_time_stamp() << endl;
        
        initiator_socket->b_transport(*trans, delay);  // Llamada de transporte bloqueante   

        // Comprobar respuesta
        if (trans->is_response_error())
          cout << name() << " GOT ERROR RESPONSE" << endl;

        delete trans;
      }
    }
    std::cout<< RED "-----------Image saved in memory---------------" ENDCOLOR <<endl;
// TEST para leer la memoria y escribir en archivo:
/*
std::ofstream output_file("output_ReadMem.txt");  // Abre el archivo para escribir datos
    if (!output_file.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        sc_stop();  // Detén la simulación si el archivo no se abre
        return;
    }

    for (unsigned int row = 0; row < newROWS; ++row) {
        for (unsigned int col = 0; col < newCOLS; ++col) {
            tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
            ID_extension* id_extension_new = new ID_extension;
            
            trans->set_extension(id_extension_new);
            id_extension_new->transaction_id = generateUniqueID();
            
            // Configurar la transacción
            trans->set_command(tlm::TLM_READ_COMMAND);  // Configura para leer
            trans->set_data_ptr(new unsigned char[1]);  // Crear buffer para un byte
            trans->set_data_length(1); // Leer un byte a la vez
            trans->set_address(row * newCOLS + col); // Dirección basada en la posición en la matriz
            
            sc_time delay = sc_time(10, SC_NS);
            cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension_new->transaction_id << " at time " << sc_time_stamp() << endl;
            
            initiator_socket->b_transport(*trans, delay);  // Llamada de transporte bloqueante   

            // Comprobar respuesta
            if (trans->is_response_error())
                cout << name() << " GOT ERROR RESPONSE" << endl;
            else {
                // Obtener el dato leído
                unsigned char data = *trans->get_data_ptr();

                // Guardar el dato en el archivo
                output_file << static_cast<int>(data) << ' ';  // Escribir el valor en el archivo
            }

            delete[] trans->get_data_ptr();  // Limpiar el buffer
            delete trans;
        }
        output_file << std::endl;  // Nueva línea para cada fila de la matriz
    }
    std::cout<<"-----------Image Saved in output.txt file---------------"<<endl;
*/
//-----------------------------
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
    transaction_pending trans_pending = {
      .trans_pending = &trans,
      .phase_pending = phase,
      .delay_pending = delay,
    };

    trans_queue.push(trans_pending);
    cv_semaphore.post();

    //Delay
    wait(delay);

    return tlm::TLM_ACCEPTED;
  }
};
