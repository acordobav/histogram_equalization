#include "ultrasonic_sensor.hpp"
#include <cmath>

#include <systemc-ams>
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
                                    double min_echo_time_us, max_echo_time_us) :
    sc_module(nm),
    tx_signal("tx_signal"),
    echo_signal("echo_signal"), 
    time_output("time_output"),
    ultrasonic_frequency(ult_frequency),
    timeTrigger(time_trigger),
    minEchoTime_us(min_echo_time_us),
    maxEchoTime_us(max_echo_time_us) {}

//Attributes
void ultrasonic_sensor::set_attributes()
{
    set_timestep(50.0, SC_MS); //recommended by device specifications
}

void ultrasonic_sensor::processing()
{
    //time-domain signal processing behavior or algorithm
    double input_signal = tx_signal.read();
    double time_now = sc_time_stamp().to_seconds();  // Get the current simulation time in seconds

    //Since distance equation (used in calc distance module) requires times in seconds
    double min_echo_time_s = minEchoTime_us * 1e-6;
    double max_echo_time_s = maxEchoTime_us * 1e-6;

    // Transmit pulse simulates the ultrasonic pulse transmitted by the sensor
    double transmit_pulse = (input_signal > 0.5) ? sin(2 * M_PI * ultrasonic_frequency * time_now) : 0.0;

    // Simulate a fixed time delay for echo signal
    // Clamp the current time to be within the specified range (we don't want distances out of the 0.2-4.5m range)
    double clamped_time = std::max(min_echo_time_s, std::min(time_now, max_echo_time_s));

    // Calculate actual time delay based on the clamped time
    // Now we are sure that the distance to be calculate later on is not going to be out of range
    // This because the sensor IS NOT able to calculate those distances
    double actual_echo_time_delay = clamped_time;

    // Simulate the echo signal with the calculated delay
    double echo_signal_value = (time_now >= actual_echo_time_delay) ? transmit_pulse : 0.0;

    // Output the simulated echo signal and the current time
    echo_signal.write(echo_signal_value);
    time_output.write(time_now);
}