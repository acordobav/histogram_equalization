#ifndef ULTRASONIC_SENSOR_HPP
#define ULTRASONIC_SENSOR_HPP

#include <systemc-ams>
#include <systemc.h>

// Following the basic structure got from "SystemC Analog/Mixed-Signal User's Guide"
// Section 2.2.1 TDF Modules, from Accellera

SCA_TDF_MODULE( ultrasonic_sensor ) {
    //Port Declarations (AMS Signals only)
    sca_tdf::sca_in<double> tx_signal;  // Señal que sera transmitida (input)
    sca_tdf::sca_out<double> echo_signal; //Señal eco
    sca_tdf::sca_out<double> time_output;   // Señal de tiempo (output)

    //Constant
    const double speed_of_sound = 343.0;  // Speed of sound in meters per second
    
    //Parameters coming directly from the sensor (i.e HC-SR04)
    double ultrasonic_frequency;  // Ultrasonic frequency in Hz (HC-SR04 - 40kHz)
    double timeTrigger;
    double minEchoTime_us;
    double maxEchoTime_us;

    //Constructor
    ultrasonic_sensor(sc_core::sc_module_name nm, 
                    double ult_frequency, double time_trigger,
                    double min_echo_time_us, double max_echo_time_us);

    //Attributes
    void set_attributes();

    void processing();

};

#endif