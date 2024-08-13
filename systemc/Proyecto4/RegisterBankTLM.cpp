#include <systemc.h>
#include "tlm.h"

struct RegisterBankTLM : sc_module {
  // TLM-2 socket
  tlm_utils::simple_target_socket<RegisterBankTLM> target_socket;

  SC_CTOR(RegisterBankTLM) : target_socket("target_socket") {
    // Registrar el método de transporte bloqueante
    target_socket.register_b_transport(this, &RegisterBankTLM::b_transport);
  }

  // Método de transporte bloqueante
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64 addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    mask_extension* ext_mask = new mask_extension;
    trans.get_extension(ext_mask);

    // Realizar la operación de lectura o escritura
    if (cmd == tlm::TLM_READ_COMMAND) {
      uint32_t data = global_register_bank.read_bits(addr,ext_mask->mask);
      memcpy(ptr, &data, len);
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
      cout << "received value: " << *ptr << endl;
      global_register_bank.write_bits(addr,ext_mask->mask,*ptr);
    }

    // Establecer el estado de la respuesta
    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    // Esperar un retraso simulado
    wait(delay);
  }
};
