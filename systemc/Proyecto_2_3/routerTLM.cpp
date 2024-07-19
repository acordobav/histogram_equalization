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
      sprintf(txt, "EqualizerTLM:target%d", i);
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
      unsigned int   len = trans_pending->get_data_length();

      // Send to initiator
      tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
      id_extension = new ID_extension;
      trans->set_extension( id_extension ); // Add the extension to the transaction

      id_extension->transaction_id = generateUniqueID();

      sc_time delay = sc_time(10, SC_NS);   

      tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);   
      trans->set_data_ptr(ptr);   
      trans->set_data_length(len);   

      cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      initiator_socket->b_transport(*trans, delay);  // Blocking transport call   

      // Initiator obliged to check response status and delay
      if ( trans->is_response_error() )
        cout << name() << " GOT ERROR RESPONSE" << endl;

      delete trans;
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
