#include "utils.hpp"

#include "memory_map.h"
#include "RegisterBank.hpp"
#include "RegisterBank.cpp"
#include "global_register_bank.hpp"

struct CamaraSensTLM: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<CamaraSensTLM> target_socket;

  // TLM2 socket, defaults to 32-bits wide, generic payload, generic DMI mode   
  tlm_utils::simple_initiator_socket<CamaraSensTLM> initiator_socket;

  // Internal data buffer used by initiator with generic payload   
  int data;

  SC_CTOR(CamaraSensTLM)   
  : target_socket("CamaraSensTLM:target"),
  initiator_socket("CamaraSensTLM:initiator")  // Construct and name socket   
  
  {   
    // Register callbacks for incoming interface method calls
    target_socket.register_nb_transport_fw(this, &CamaraSensTLM::target_nb_transport_fw);
    initiator_socket.register_nb_transport_bw(this, &CamaraSensTLM::initiator_nb_transport_bw);
    
    //Inst new CAMARA SENSOR

    SC_THREAD(thread_process);   
  }   

  void thread_process()   
  {
    tlm::tlm_sync_enum status;

    while(true){
        // Wait for an event to pop out of the back end of the queue   
        wait(e1);

        ID_extension* id_extension = new ID_extension;
        trans_pending->get_extension(id_extension); 

        // Obliged to set response status to indicate successful completion   
        trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);  

        cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;
        
        //Variables to store the incoming data
        bool sens_active = false;

        if (global_register_bank.read_bits(REG_BASE_2+0x2,0x1)){
            unsigned char* data_ptr = trans_pending->get_data_ptr();
            memcpy(&sens_active, data_ptr, sizeof(bool));

            cout << "From TARGET CAM SENSOR" << endl;
            cout << "Sens active bool coming from sensor : " << sens_active << endl;

            cout << "FROM REGISTER STATUS OF SENS_ACTIVE: " << global_register_bank.read_bits(REG_BASE_2+0x2,0x1) << endl;

            //Since camara sensor was actived, now disabling it for next iteration
            global_register_bank.write_bits(REG_BASE_2+0x2,0x1,0x0)
        }

        tlm::tlm_phase phase = tlm::BEGIN_RESP;
        status = target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);

        // Check value returned from nb_transport   
        if (status != tlm::TLM_ACCEPTED) {
        cout << name() << " unknown response TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;
        }

        // Process DATA coming from Sensor
        if (sens_active){
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
        }

        // TLM2 generic payload transaction
        tlm::tlm_generic_payload trans;
        ID_extension* id_extension = new ID_extension;
        trans.set_extension( id_extension ); // Add the extension to the transaction

        id_extension->transaction_id = generateUniqueID();

        tlm::tlm_phase phase = tlm::BEGIN_REQ;   
        sc_time delay = sc_time(10, SC_NS);   

        //Data to be sent        
        tlm::tlm_phase phase = tlm::BEGIN_REQ;   
        sc_time delay = sc_time(10, SC_NS);   
                
        tlm::tlm_command cmd = static_cast<tlm::tlm_command>(rand() % 2);   
        trans.set_data_ptr( reinterpret_cast<unsigned char*>(image) );   
        trans.set_data_length( sizeof(image) );   

        // Other fields default: byte enable = 0, streaming width = 0, DMI_hint = false, no extensions   
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