#ifndef ULTRASONIC_SENSOR_HPP
#define ULTRASONIC_SENSOR_HPP

#include "systemc.h"
#include "systemc-ams.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdlib> //for random values purposes

// Following the basic structure got from "SystemC Analog/Mixed-Signal User's Guide"
// Section 2.2.1 TDF Modules, from Accellera

SCA_TDF_MODULE(ultrasonic_sensor) {
    //Port Declarations (AMS Signals only)
    sca_tdf::sca_in<double> tx_signal;  // Señal que sera transmitida (input)
    sca_tdf::sca_out<double> echo_out; //Señal eco
    sca_tdf::sca_out<double> time_delay;   // Señal de tiempo (output)

    //Constant
    const double speed_of_sound = 343.0;  // Speed of sound in meters per second
    
    //Parameters coming directly from the sensor (i.e HC-SR04)
    double ultrasonic_frequency;  // Ultrasonic frequency in Hz (HC-SR04 - 40kHz)
    double timeTrigger;
    double minEchoTime_us;
    double maxEchoTime_us;
    double elapsed_time;
    double last_echo;
    bool detected;
    double detection_delay;

    //Constructor
    ultrasonic_sensor(sc_core::sc_module_name nm, 
                    double ult_frequency, double time_trigger,
                    double min_echo_time_us, double max_echo_time_us);

    //Attributes
    void set_attributes();

    void processing();

    double detect(double input_signal);

    void read_outputs();

    double read_last_echo();

    double read_detection_elapsed_time();
};

#endif