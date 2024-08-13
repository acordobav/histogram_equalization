#include <systemc.h>
#include "tlm.h"

struct RegisterBankTLM : sc_module
{
  // TLM-2 socket
  tlm_utils::simple_target_socket<RegisterBankTLM> target_socket;

  SC_CTOR(RegisterBankTLM) : target_socket("target_socket")
  {
    // Registrar el método de transporte bloqueante
    target_socket.register_b_transport(this, &RegisterBankTLM::b_transport);
  }

  // Metodo de transporte bloqueante
  virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay)
  {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64 addr = trans.get_address();
    void *ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    // Realizar la operacion de lectura o escritura
    if (cmd == tlm::TLM_READ_COMMAND)
    {
      uint32_t data = global_register_bank.read(addr, len);
      memcpy(ptr, &data, len);
    }
    else if (cmd == tlm::TLM_WRITE_COMMAND)
    {
      global_register_bank.write(addr, ptr, len);
    }

    // Establecer el estado de la respuesta
    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    // Esperar un retraso simulado
    wait(delay);
  }
};
