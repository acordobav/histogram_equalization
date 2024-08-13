#include "memory_map.h"
#include "original_image.hpp"
#include <cstdlib>

class signal_driver2 : public sca_tdf::sca_module
{
public:
  sca_tdf::sca_out<bool> out;

  signal_driver2(sc_core::sc_module_name nm) : out("out"), signal_value(0.0) {}

  void set_attributes()
  {
    set_timestep(1.0, SC_MS); // Set the time step to 1 ms
  }

  void processing()
  {
    out.write(signal_value);
  }

  void set_signal_value(double value)
  {
    signal_value = value;
  }

private:
  double signal_value;
};

struct CamaraSensTLM : sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<CamaraSensTLM> target_socket;

  // TLM2 socket, defaults to 32-bits wide, generic payload, generic DMI mode
  tlm_utils::simple_initiator_socket<CamaraSensTLM> initiator_socket;
  tlm_utils::simple_initiator_socket<CalcDistTLM> register_socket;

  // Internal data buffer used by initiator with generic payload
  sc_event e1;
  tlm::tlm_generic_payload *trans_pending;
  tlm::tlm_phase phase_pending;
  sc_time delay_pending;

  // Internal data buffer used by initiator with generic payload

  camara_sensor *cs1;
  signal_driver2 *sig_drv;
  // CalcDistTLM* calc_dist_TLM_Mod;  //Instanciando modulo
  // dist_calc* calcDistMod;

  sc_signal<bool> sens_active;
  // using namespace sc_core;
  double const_digitalization = 190;
  double tiempo;

  sc_signal<bool> sens_active_in;
  sca_tdf::sca_signal<double> echo_signal_cable;
  sca_tdf::sca_signal<double> time_signal_cable;
  sc_signal<bool> digital_image_ready;
  int data;
  int tmp;
  bool sens_active_result;
  bool digital_image_result;

  SC_CTOR(CamaraSensTLM)
    : target_socket("CamaraSensTLM:target")
    , initiator_socket("CamaraSensTLM:initiator") // Construct and name socket
    , register_socket("CamaraSensTLM:register")
  {
    target_socket.register_nb_transport_fw(this, &CamaraSensTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &CamaraSensTLM::initiator_nb_transport_bw);

    cs1 = new camara_sensor("cs1", const_digitalization, tiempo);
    cs1->sens_active_in(sens_active_in);
    cs1->digital_image_ready(digital_image_ready);

    SC_THREAD(thread_process);
  }

  void register_data_set(uint64_t address, uint32_t value)
  {
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr((uint8_t *)&value);
    trans.set_data_length(sizeof(uint32_t));
    trans.set_address(address);

    register_socket->b_transport(trans, delay);
  }

  void thread_process()
  {
    tlm::tlm_sync_enum status;

    while (true)
    {
      // Wait for an event to pop out of the back end of the queue
      wait(e1);
      ID_extension *id_extension = new ID_extension;
      trans_pending->get_extension(id_extension);

      // Obliged to set response status to indicate successful completion
      trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);

      cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;

      unsigned char *data_ptr = trans_pending->get_data_ptr();
      memcpy(&tmp, data_ptr, sizeof(int));
      sens_active_in.write(true);
      wait(1000, SC_US);
      digital_image_result = digital_image_ready.read();
      wait(1, SC_US);
      uint8_t **imagen = createMatrix(ROWS, COLS);
      if (digital_image_result)
      {
        int index = 0;
        // EXtracting the red channel
        for (int i = 0; i < ROWS; i++)
        {
          for (int j = 0; j < COLS; j++)
          {
            imagen[i][j] = original_image[index];
            index++;
          }
        }
        wait(20, SC_US);
      }

      tlm::tlm_phase phase = tlm::BEGIN_RESP;
      //  status = target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);
      // if (status != tlm::TLM_ACCEPTED) {
      //    cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      // }

      // TLM2 generic payload transaction
      tlm::tlm_generic_payload trans;
      id_extension = new ID_extension;
      trans.set_extension(id_extension); // Add the extension to the transaction
      id_extension->transaction_id = generateUniqueID();
      phase = tlm::BEGIN_REQ;
      sc_time delay = sc_time(10, SC_NS);

      trans.set_data_length(sizeof(imagen));
      trans.set_data_ptr((unsigned char*)imagen);

      /*
      //------- FUNCIONAL ENVIO IMAGEN ----
      trans.set_data_length( sizeof(image_digitalized) );
        unsigned char data[sizeof(image_digitalized)];
          memcpy(data, &image_digitalized, sizeof(data));
          trans.set_data_ptr(data);
      //------- FUNCIONAL ENVIO IMAGEN ----
      */

      cout << name() << " BEGIN_REQ SENT" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      status = initiator_socket->nb_transport_fw(trans, phase, delay); // Non-blocking transport call

      // Check value returned from nb_transport
      if (status != tlm::TLM_ACCEPTED)
      {
        cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
      }
    }
  }

  // TLM-2 non-blocking transport method
  virtual tlm::tlm_sync_enum target_nb_transport_fw(
      tlm::tlm_generic_payload &trans,
      tlm::tlm_phase &phase,
      sc_time &delay)
  {
    ID_extension *id_extension = new ID_extension;
    trans.get_extension(id_extension);

    if (phase != tlm::BEGIN_REQ)
    {
      cout << name() << " unknown phase " << phase << endl;
      return tlm::TLM_ACCEPTED;
    }

    cout << name() << " BEGIN_REQ RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;

    // Now queue the transaction until the annotated time has elapsed
    trans_pending = &trans;
    phase_pending = phase;
    delay_pending = delay;

    e1.notify();

    // Delay
    wait(delay);

    return tlm::TLM_ACCEPTED;
  }

  // ************************************************
  // TLM2 backward path non-blocking transport method
  // ************************************************
  virtual tlm::tlm_sync_enum initiator_nb_transport_bw(
      tlm::tlm_generic_payload &trans,
      tlm::tlm_phase &phase,
      sc_time &delay)
  {
    ID_extension *id_extension = new ID_extension;
    trans.get_extension(id_extension);

    if (phase != tlm::BEGIN_RESP)
    {
      cout << name() << " unknown phase " << phase << endl;
      return tlm::TLM_ACCEPTED;
    }

    // Initiator obliged to check response status
    if (trans.is_response_error())
      SC_REPORT_ERROR("TLM2", "Response error from nb_transport");
    // Delay
    wait(delay);

    cout << name() << " BEGIN_RESP RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;

    return tlm::TLM_ACCEPTED;
  }
};
