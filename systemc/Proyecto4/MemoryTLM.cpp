#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

struct MemoryTLM : sc_module {
  // TLM-2 socket
  tlm_utils::simple_target_socket<MemoryTLM> target_socket;

  // Tamaño de la memoria
  enum { SIZE = 32768 }; // Ajusta el tamaño según sea necesario
  unsigned char mem[SIZE];

  SC_CTOR(MemoryTLM) : target_socket("target_socket") {
    // Registrar el método de transporte bloqueante
    target_socket.register_b_transport(this, &MemoryTLM::b_transport);
  }

  // Método de transporte bloqueante
  virtual void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64 addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    unsigned int len = trans.get_data_length();

    // Verificar si la dirección y longitud de los datos son válidas
    if (addr >= SIZE || len > SIZE) {
      SC_REPORT_ERROR("TLM-2", "Target does not support given generic payload transaction");
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
      return;
    }

    // Realizar la operación de lectura o escritura
    if (cmd == tlm::TLM_READ_COMMAND) {
      memcpy(ptr, &mem[addr], len);
      std::cout<<"READ Adress:"<<addr<<std::endl;
    } else if (cmd == tlm::TLM_WRITE_COMMAND) {
      memcpy(&mem[addr], ptr, len);
      std::cout<<"WRITE Adress:"<<addr<<std::endl;
    }

    // Establecer el estado de la respuesta
    trans.set_response_status(tlm::TLM_OK_RESPONSE);

    // Esperar un retraso simulado
    wait(delay);

    // Mostrar información de la transacción
    ID_extension* id_extension = new ID_extension;
    trans.get_extension(id_extension);
    cout << name() << " BEGIN_REQ RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
  }
};
