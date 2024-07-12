// Needed for the simple_target_socket
#define SC_INCLUDE_DYNAMIC_PROCESSES

#include <systemc.h>
#include <systemc-ams>

// Required because of functionalities of the modules
#include <iostream>
#include <string>
#include <cmath>

// Required because of implemented modules/registers
#include "memory_map.h"
#include "RegisterBank.hpp"
#include "ultrasonicSensor.hpp"

using namespace sc_core;   
using namespace sc_dt;   
using namespace std;   
   
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"

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

/*--------------------------------------------------------------------*/
// FUNCIONES GENERALES
/*
La siguiente funcion sirve para mantener un "track" del numero de
transaccion en la que se encuentra el proceso, entre otros

- Podria no ser necesario (temp*)
*/
struct ID_extension: tlm::tlm_extension<ID_extension> {
  ID_extension() : transaction_id(0) {}
  virtual tlm_extension_base* clone() const { // Must override pure virtual clone method
    ID_extension* t = new ID_extension;
    t->transaction_id = this->transaction_id;
    return t;
  }

  // Must override pure virtual copy_from method
  virtual void copy_from(tlm_extension_base const &ext) {
    transaction_id = static_cast<ID_extension const &>(ext).transaction_id;
  }
  unsigned int transaction_id;
};

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
 


/*---------------------------------------------------------------------*/
// BLOQUE: TOP

SC_MODULE(Top)   
{   
  InitiatorSensProx    *initiatorSensProx; 
  InitiatorDetector    *initiatorDetector;
  InitiatorCamara      *initiatorCamara;
  InitiatorEcualizador *initiatorEcualizador;
  InitiatorCompresor   *initiatorCompresor;

  TargetSensProx       *targetSensProx; 
  TargetDetector       *targetDetector;
  TargetCamara         *targetCamara;
  TargetEcualizador    *targetEcualizador;
  TargetCompresor      *targetCompresor;

  RouterT1<8>          *routerT1;
  RouterT2<2>          *routerT2;

  Memory               *memory; 

  //For the AMS module
  //Ultrasonic sensor
  sca_tdf::sca_signal<double> tx_signal, echo_signal, time_output;
  int num_samples;

  ultrasonic_sensor    *sensor;
   
  SC_CTOR(Top) : num_samples(100) //The quantity of samples to be simulated   
  {   
    // Instantiate components 

    // SENSOR PROXIMIDAD
    string strSensProx = "init_SensProx";
    for (int i = 0; i < 8; i++)
    {
      sprintf("InitiatorSensProx_%d", i);
      initSensProx[i] = new InitiatorSensProx(strSensProx+to_string(i));
    }

    string strTargetSensProx = "target_SensProx";
    for (int i = 0; i < 8; i++)
    {
      sprintf("TargetSensProx_%d", i);
      targetSensProx[i] = new TargetSensProx(strTargetSensProx+to_string(i));
    }

    // DETECTOR
    string strDetector = "init_Detector";
    for (int i = 0; i < 8; i++)
    {
      sprintf("InitiatorDetector_%d", i);
      initDetector[i] = new InitiatorDetector(strDetector+to_string(i));
    }

    string strTargetDetector = "target_Detector";
    for (int i = 0; i < 8; i++)
    {
      sprintf("TargetDetector_%d", i);
      targetDetector[i] = new TargetDetector(strTargetDetector+to_string(i));
    }

    // CAMARA
    string strCamara = "init_Camara";
    for (int i = 0; i < 8; i++)
    {
      sprintf("InitiatorCamara_%d", i);
      initCamara[i] = new InitiatorCamara(strCamara+to_string(i));
    }

    string strTargetCamara = "target_Camara";
    for (int i = 0; i < 8; i++)
    {
      sprintf("TargetCamara_%d", i);
      targetCamara[i] = new TargetCamara(strTargetCamara+to_string(i));
    }

    // ECUALIZADOR
    initEcualizador = new InitiatorEcualizador("init_Ecualizador");

    targetEcualizador = new TargetEcualizador("target_Ecualizador");

    // COMPRESOR
    initCompresor = new InitiatorCompresor("init_Compresor");

    targetCompresor = new TargetCompresor("target_Compresor");

    // ROUTER
    routerT1_Camara = new RouterT1<8>("routerT1_Camara");

    routerT1_Est = new RouterT1<8>("routerT1_Est");

    routerMemory = new RouterT2<2>("routerMemory");

    // MEMORIA
    memory = new Memory   ("memory");

    // AMS MODULES
    // ULTRASONIC SENSOR
    ultrasonicSensor = new ultrasonic_sensor("ultrasonicSensor", 40.0, 10, 100, 25000);
    ultrasonicSensor->tx_signal(tx_signal);
    ultrasonicSensor->echo_signal(echo_signal);
    ultrasonicSensor->time_output(time_output);

    SC_THREAD(stimulus);
   
   /*------------------------------------------------------------------*/
    // Bind initiator socket to target socket
    // See references to each "Conexion" at the beginning of the script
    //Conexion 1
    for (int i = 0; i < 8; i++)
      initSensProx[i]->initiator_socket.bind( targetDetector[i]->target_socket );

    //Conexion 2
    for (int i = 0; i < 8; i++)
      initDetector[i]->initiator_socket[0].bind( targetCamara[i]->target_socket );

    //Conexion 3
    for (int i = 0; i < 8; i++)
      initCamara[i]->initiator_socket.bind( routerT1_Camara->target_socket[i] );

    //Conexion 4
    for (int i = 0; i < 8; i++)
      initDetector[i]->initiator_socket[1].bind( routerT1_Est->target_socket[i] );
    
    //Conexion 5
    for (int i = 0; i < 4; i++)
      routerT1_Camara->initiator_socket->bind( targetEcualizador->target_socket );

    //Conexion 6
    initEcualizador->initiator_socket.bind( targetCompresor->target_socket );

    //Conexion 7
    initCompresor->initiator_socket.bind( routerMemory->target_socket[0] );
    routerT1_Est->initiator_socket->bind( routerMemory->target_socket[1] );

    //Conexion 8
    routerMemory->initiator_socket.bind( memory->target_socket );
  }

  void stimulus()
  {
    for (int i = 0; i < num_samples; ++i) 
    {
      // Simulate a pulse transmission
      double pulse = 1.0;
      tx_signal.write(pulse);
      wait(10, SC_MS);

      pulse = 0.0;
      tx_signal.write(pulse);
      wait(50, SC_MS); //To match suggested wait time of the actual sensor
    }
  }

};

/*---------------------------------------------------------------------*/
// MAIN

int sc_main(int argc, char* argv[])
{
  //Create the Registers Bank according to the Memory Bank
  RegisterBank reg_bank(0x10000, 0x10072);

  //To initiate the different modules
  Top top("top");
  sc_start();
  return 0;
}