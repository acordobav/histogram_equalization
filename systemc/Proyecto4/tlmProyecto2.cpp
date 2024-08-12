// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

// SYSTEMC related
#include "utils.hpp"
#include <systemc.h>
#include <systemc-ams>

#include "dist_calc.cpp"
#include "camara_sensor.cpp"
#include "equalization.cpp"

#include "interpolation.cpp"

#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

// TLM MODULES
// Ultrasonic Sensor
#include "ultrasonicSensorTLM.cpp"
#include "ultrasonicSensor.cpp"
// CALC_DIST
#include "calc_dist_TLM.cpp"
//CAMARA SENSOR
#include "camara_sensor_TLM.cpp"
// EQUALIZER
#include "equalizerTLM.cpp"
// INTERPOLATION
#include "InterpolationTLM.cpp"
// ROUTER
#include "routerTLM.cpp"
// MEMORY
#include "MemoryTLM.cpp"

// Required because of implemented registers
#include "memory_map.h"
#include "RegisterBank.hpp"
#include "RegisterBank.cpp"
#include "global_register_bank.hpp"
#include "utils.hpp"

// Required because of functionalities of the modules
#include <iostream>
#include <string>
#include <cmath>

using namespace sc_core;   
using namespace sc_dt;   
using namespace std;   
   


/*------------------------------------------------------------------------------------------*/
//Global BANK OF REGISTERS
//Need to remove it in the future
RegisterBank global_register_bank(0x10000, 0x60000000);

/*------------------------------------------------------------------------------------------*/
/*
Bloques necesarios para el proyecto
- Iniciadores
- Targets
- Routers
- Memoria

Adicionalmente, se constituyen las siguientes conexiones basicas:
(Para mas detaller referirse al diagrama de conexiones)

Conexion 1:
Sensor Proximidad[0-7] (AMS)(Initiator) ----> Detector[0-7] (Target)

Conexion 2:
Detector[0-7] (Initiator) ------------------> Camara[0-7] (AMS)(Target)

Conexion 3: 
Camara[0-7] (AMS)(Initiator) ---------------> Router1

Conexion 4: 
Detector[0-7] (Initiator) ------------------> Router2

Conexion 5:
Router1 ------------------------------------> Ecualizador (Target)

Conexion 6:
Ecualizador (Initiator) --------------------> Compresor (Target)

Conexion 7:
Compresor (Initiator) ----------------------> Router3
Router2 ------------------------------------> Router3

Conexion 8:
Router3 ------------------------------------> Memoria
--------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
// BLOQUE: SENSOR PROXIMIDAD



/*---------------------------------------------------------------------*/
// BLOQUE: DETECTOR



/*---------------------------------------------------------------------*/
// BLOQUE: CAMARA



/*---------------------------------------------------------------------*/
// BLOQUE: ECUALIZADOR



/*---------------------------------------------------------------------*/
// BLOQUE: COMPRESOR



/*---------------------------------------------------------------------*/
// BLOQUE: ROUTERS



/*---------------------------------------------------------------------*/
// BLOQUE: MEMORIA

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
	//cout << "LAST TARGET MODULE" << endl << endl;
    ID_extension* id_extension = new ID_extension;
    trans_pending->get_extension(id_extension); 

    // Obliged to set response status to indicate successful completion   
    trans_pending->set_response_status(tlm::TLM_OK_RESPONSE);  
    wait(200,SC_MS);
    cout << "LAST TARGET MODULE" << endl << endl;
    cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;
    
    unsigned char* filtered_image = trans_pending->get_data_ptr();

    // Save output image
    stbi_write_jpg("filtered.jpg", COLS/2, ROWS/2, 1, filtered_image, (ROWS/2)*(COLS/2));

    free(filtered_image);

    // Call on backward path to complete the transaction
    tlm::tlm_phase phase = tlm::BEGIN_RESP;
    target_socket->nb_transport_bw(*trans_pending, phase, delay_pending);
     sc_stop();
  }
}

  // TLM-2 non-blocking transport method
  virtual tlm::tlm_sync_enum nb_transport_fw(
    tlm::tlm_generic_payload& trans,
    tlm::tlm_phase& phase,
    sc_time& delay)
  {
  //cout << name() << "LAST TARGET MODULE!!!!!!!!!!!!!" << endl << endl;
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
/*
struct BlockingTarget: sc_module
{
  // TLM-2 socket, defaults to 32-bits wide, base protocol
  tlm_utils::simple_target_socket<BlockingTarget> target_socket;

  // Construct and name socket   
  SC_CTOR(BlockingTarget)
  : target_socket("BlockingTarget:target")
  {
    // Register callbacks for incoming interface method calls
    target_socket.register_b_transport(this, &BlockingTarget::b_transport);
  }

  // TLM-2 non-blocking transport method
  virtual void b_transport( tlm::tlm_generic_payload& trans, sc_time& delay )
  {
    ID_extension* id_extension = new ID_extension;
    trans.get_extension(id_extension);

    cout << name() << " BEGIN_REQ RECEIVED" << " TRANS ID " << id_extension->transaction_id << " at time " << sc_time_stamp() << endl;      

    //Delay
    wait(delay);

    cout << name() << " BEGIN_RESP SENT" << " TRANS ID " << id_extension->transaction_id <<  " at time " << sc_time_stamp() << endl;
    
    // Obliged to set response status to indicate successful completion
    trans.set_response_status( tlm::TLM_OK_RESPONSE );
  }
};

*/
/*---------------------------------------------------------------------*/
// BLOQUE: TOP

#define LANES 8
SC_MODULE(Top)
{
  //InitiatorSensProx       *initiatorSensProx;
  UltrasonicSensorTLM*      uSensor[LANES];
  CalcDistTLM*              calcDist[LANES];
  CamaraSensTLM*            cSens[LANES];
  EqualizerTLM*             equalizer[LANES];
  InterpolationTLM*         interpolation[LANES];
  RouterTLM<LANES>*         router;
  //BlockingTarget*           blockingTarget;
  MemoryTLM*                memory; 

  SC_CTOR(Top) //The quantity of samples to be simulated   
  {   
    // Instantiate components 

    // SENSOR PROXIMIDAD
    char strDefault[50];
    for (int i = 0; i < LANES; i++)
    {
      sprintf(strDefault,RED "UltrasonicSensor_%d" ENDCOLOR, i);
      uSensor[i] = new UltrasonicSensorTLM(strDefault);
    }

    // DETECTOR
    for (int i = 0; i < LANES; i++)
    {
      sprintf(strDefault,GREEN "CalcDistModule_%d" ENDCOLOR, i);
      calcDist[i] = new CalcDistTLM(strDefault);
    }

    // CAMARA
    for (int i = 0; i < LANES; i++)
    {
      sprintf(strDefault,YELLOW "CamaraSensor_%d" ENDCOLOR, i);
      cSens[i] = new CamaraSensTLM(strDefault);
    }  

   // ECUALIZADOR
   for (int i = 0; i < LANES; i++)
    {
      sprintf(strDefault,BLUE "equalizerTLM_%d" ENDCOLOR, i);
      equalizer[i] = new EqualizerTLM(strDefault);
    }

    // COMPRESOR
    
    for (int i = 0; i < LANES; i++)
    {
      sprintf(strDefault,CYAN "InterpolationTlM%d" ENDCOLOR, i);
      interpolation[i] = new InterpolationTLM(strDefault);
    }
    
    // ROUTER
    router = new RouterTLM<LANES>(MAGENTA "routerTLM" ENDCOLOR);

    
    // MEMORIA
    memory = new MemoryTLM   (TEAL "memory" ENDCOLOR);
    

    //Temp Solution
    //blockingTarget = new BlockingTarget("blockingTarget");
   
   /*------------------------------------------------------------------*/
    // Bind initiator socket to target socket
    // See references to each "Conexion" at the beginning of the script
    //Conexion 1
    for (int i = 0; i < LANES; i++)
      uSensor[i]->initiator_socket.bind( calcDist[i]->target_socket );

    //Conexion 2
    for (int i = 0; i < LANES; i++)
      calcDist[i]->initiator_socket.bind( cSens[i]->target_socket );

    //Conexion 3
    for (int i = 0; i < LANES; i++)
      cSens[i]->initiator_socket.bind(equalizer[i]->target_socket );

    //Conexion 4
    for (int i = 0; i < LANES; i++)
      equalizer[i]->initiator_socket.bind(interpolation[i]->target_socket);

    //Conexion 5
    for (int i = 0; i < LANES; i++)
      interpolation[i]->initiator_socket.bind(*router->target_socket[i]);

    //Conexion 6
    // Router con memoria
    router->initiator_socket.bind(memory->target_socket);

  }
};

/*---------------------------------------------------------------------*/
// MAIN

int sc_main(int argc, char* argv[])
{
  //Create the Registers Bank according to the Memory Bank
  //RegisterBank reg_bank(0x10000, 0x10072);

  //To initiate the different modules
  Top top("top");
  sc_start();
  return 0;
}
