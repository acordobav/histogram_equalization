#include "ultrasonicSensor.hpp"
#include <cmath>
#include <algorithm>

#include "systemc.h"
#include "systemc-ams.h"
#include <cstdlib> //for random values purposes

/*
Context
Distancia se puede calcular utilizando la siguiente formula
Distancia(m) = (Tiempo del pulso ECO) * (Velocidad del sonido) / 2

Dispositivo: HC-SR04
Frecuencia de ultrasonido: 40KHz
Duración mínima del pulso de disparo TRIG (nivel TTL): 10 μS
Duración del pulso ECO de salida (nivel TTL): 100-25000 μS
Rango de medición: 2cm a 450cm (0.02m - 4.5m)
Tiempo mínimo de espera entre una medida y el inicio de otra 20ms (recomendable 50ms)
*/

//Constructor
ultrasonic_sensor::ultrasonic_sensor(sc_core::sc_module_name nm, 
                                    double ult_frequency, double time_trigger,
                                    double min_echo_time_us, double max_echo_time_us) :
    tx_signal("tx_signal"),
    echo_out("echo_out"), 
    time_delay("time_delay"),
    ultrasonic_frequency(ult_frequency),
    timeTrigger(time_trigger),
    elapsed_time(0.0),
    detection_delay(0.0),
    last_echo(0.0),
    detected(false),
    minEchoTime_us(min_echo_time_us),
    maxEchoTime_us(max_echo_time_us) {}

//Attributes
void ultrasonic_sensor::set_attributes()
{
    set_timestep(1.0, SC_MS); //recommended by device specifications
}

void ultrasonic_sensor::processing()
{
    //time-domain signal processing behavior or algorithm
    double input_signal = tx_signal.read();
    double echo_value = detect(input_signal);

    if (echo_value != 0.0) {
        double time_now = elapsed_time; // Get the elapsed simulation time in seconds

        //Since distance equation (used in calc distance module) requires times in seconds
        double min_echo_time_s = minEchoTime_us * 1e-6;
        double max_echo_time_s = maxEchoTime_us * 1e-6;

        // Simulate a fixed time delay for echo signal
        // Clamp the current time to be within the specified range (we don't want distances out of the 0.2-4.5m range)
        double clamped_time = std::max(min_echo_time_s, std::min(time_now, max_echo_time_s));

        // Output the simulated echo signal and the current time
        echo_out.write(echo_value);
        time_delay.write(clamped_time);

        last_echo = echo_value;
    }
}

double ultrasonic_sensor::detect(double input_signal) 
{
    double echo_value = 0.0;

    //std::cout << "Detection delay: " << detection_delay << " Input Signal: " << input_signal << std::endl;
    if (detection_delay <= 0.0 && input_signal == 1 && !detected) {
        detected = true;
        double time_now = sc_time_stamp().to_seconds();
        echo_value = std::sin(2 * M_PI * ultrasonic_frequency * time_now);
    }
    else if (detection_delay <= 0.0) {
        detected = false;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> pulse_width(1.16, 23.33);

        double pulse_width_dist = pulse_width(gen);

        elapsed_time = pulse_width_dist * 0.001;
        detection_delay = elapsed_time;
    }
    else if (detection_delay > 0.0 && input_signal == 1) {
        detection_delay -= 0.001;
    }

    return echo_value;
}

void ultrasonic_sensor::read_outputs() 
{
    std::cout << "Echo Signal: " << last_echo << " Time Diff Output: " << elapsed_time << std::endl;
}

double ultrasonic_sensor::read_last_echo() 
{
    return last_echo;
}

double ultrasonic_sensor::read_detection_elapsed_time() 
{
    return elapsed_time;
}
