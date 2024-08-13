#include "utils.hpp"
#include <queue>

template <unsigned int N_TARGETS>
struct BlockingRouterTLM : sc_module
{
  // List of target sockets
  tlm_utils::simple_target_socket<BlockingRouterTLM>* target_socket[N_TARGETS];

  // Initiator socket
  tlm_utils::simple_initiator_socket_tagged<BlockingRouterTLM> initiator_socket;

  // Mutex that locks the initiator socket so only one thread can access it
  sc_mutex cv_mutex;

  SC_CTOR(BlockingRouterTLM)
      : initiator_socket("BlockingRouterTLM:initiator")
      , cv_mutex() // Initialize the mutex
  {
    // Register callbacks for incoming interface method calls
    for (unsigned int i = 0; i < N_TARGETS; i++)
    {
      char txt[30];
      sprintf(txt, "BlockingRouterTLM:target%d", i);
      target_socket[i] = new tlm_utils::simple_target_socket<BlockingRouterTLM>(txt);
      target_socket[i]->register_b_transport(this, &BlockingRouterTLM::target_b_transport);
    }
  }

  // TLM-2 blocking transport method
  virtual void target_b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    // Lock the mutex so only one thread can access the initiator socket
    cv_mutex.lock();

    initiator_socket->b_transport(trans, delay);

    // Unlock the mutex to free the initiator socket for the next thread
    cv_mutex.unlock();

	  trans.set_response_status(tlm::TLM_OK_RESPONSE);
  }
};
