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
// ROUTERS
#include "routerTLM.cpp"
#include "blockingRouterTLM.cpp"
// MEMORY
#include "MemoryTLM.cpp"
// REGISTER BANK
#include "RegisterBankTLM.cpp"

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

/*---------------------------------------------------------------------*/
// BLOQUE: TOP

#define LANES 1
#define NUM_MODULES 5
#define REGBANK_T (LANES * NUM_MODULES)
SC_MODULE(Top)
{
  UltrasonicSensorTLM*          uSensor[LANES];
  CalcDistTLM*                  calcDist[LANES];
  CamaraSensTLM*                cSens[LANES];
  EqualizerTLM*                 equalizer[LANES];
  InterpolationTLM*             interpolation[LANES];
  RouterTLM<LANES>*             router;
  BlockingRouterTLM<REGBANK_T>* blockingRouter;
  MemoryTLM*                    memory; 
  RegisterBankTLM*              registerBank;


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
    
    // ROUTERS
    router = new RouterTLM<LANES>(MAGENTA "routerTLM" ENDCOLOR);
    blockingRouter = new BlockingRouterTLM<REGBANK_T>("blockingRouterTLM");

    // MEMORIA
    memory = new MemoryTLM(TEAL "memory" ENDCOLOR);
    
    // REGISTER BANK
    registerBank = new RegisterBankTLM("registerBank");

    /*------------------------------------------------------------------*/
    uint32_t regbank_i = 0;

    // Bind initiator socket to target socket
    // See references to each "Conexion" at the beginning of the script
    //Conexion 1
    for (int i = 0; i < LANES; i++) {
      uSensor[i]->initiator_socket.bind( calcDist[i]->target_socket );
      uSensor[i]->register_socket.bind(*blockingRouter->target_socket[regbank_i]);
      regbank_i++;
    }

    //Conexion 2
    for (int i = 0; i < LANES; i++) {
      calcDist[i]->initiator_socket.bind( cSens[i]->target_socket );
      calcDist[i]->register_socket.bind(*blockingRouter->target_socket[regbank_i]);
      regbank_i++;
    }

    //Conexion 3
    for (int i = 0; i < LANES; i++) {
      cSens[i]->initiator_socket.bind(equalizer[i]->target_socket );
      cSens[i]->register_socket.bind(*blockingRouter->target_socket[regbank_i]);
      regbank_i++;
    }

    //Conexion 4
    for (int i = 0; i < LANES; i++) {
      equalizer[i]->initiator_socket.bind(interpolation[i]->target_socket);
      equalizer[i]->register_socket.bind(*blockingRouter->target_socket[regbank_i]);
      regbank_i++;
    }

    //Conexion 5
    for (int i = 0; i < LANES; i++) {
      interpolation[i]->initiator_socket.bind(*router->target_socket[i]);
      interpolation[i]->register_socket.bind(*blockingRouter->target_socket[regbank_i]);
      regbank_i++;
    }

    //Conexion 6
    // Router con memoria
    router->initiator_socket.bind(memory->target_socket);

    blockingRouter->initiator_socket.bind(registerBank->target_socket);
  }
};

/*---------------------------------------------------------------------*/
// MAIN

int sc_main(int argc, char* argv[])
{
  //Create the Registers Bank according to the Memory Bank
  //RegisterBank reg_bank(0x10000, 0x10072);

  global_register_bank.write_bits(REG_COLS,0xFFFFFFFF,COLS);
  global_register_bank.write_bits(REG_ROWS,0xFFFFFFFF,ROWS);

  //To initiate the different modules
  Top top("top");
  sc_start();
  return 0;
}
