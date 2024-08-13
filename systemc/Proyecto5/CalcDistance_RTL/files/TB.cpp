// TB.cpp
#include <systemc.h>
#include <cmath>  // Para fabs
#include "CalsDis.h" // Incluye solo el encabezado

SC_MODULE(Testbench) {
    sc_out<int> all_tests_passed;  // Cambiado a sc_signal
    sc_out<int> distance;
    sc_out<double> voltaje;
    sc_out<int> sensor;

    void run_tests() {
        int dist_cm;
        double calc_voltage;
        int sens_range;

        // Mediciones a realizar
        int expected_dist[] = {10, 50, 120, 180, 220, 280, 350, 320};

        // Mediciones esperadas para el "self-checking"
        double echo_times[] = {
            10 / 0.034, 50 / 0.034, 120 / 0.034, 180 / 0.034,
            220 / 0.034, 280 / 0.034, 350 / 0.034, 320 / 0.034};
        double expected_voltage[] = {0.016, 0.080, 0.192, 0.288, 0.352, 0.448, 0.560, 0.512};
        int expected_range[] = {1, 1, 2, 2, 3, 3, 0, 0};

        // Flags para el "self-checking" y tolerancias
        int all_tests_passed_val = 1;
        const int toleranceDistance = 0;
        const double toleranceVoltage = 0.005;

        // Inicio de las mediciones
        const int num_measurements = sizeof(echo_times) / sizeof(double);
        const int trigger=1;
        for (int i = 0; i < num_measurements; ++i) {

            // Llamada al módulo
            top(trigger, echo_times[i], &dist_cm, &calc_voltage, &sens_range);

            // Visualización en Consola
            printf("Medición número: %d\n", i + 1);
            printf("Distance (cm): %d\n", dist_cm);
            printf("Voltage: %.3f\n", calc_voltage);

            switch (sens_range) {
                case 1:
                    printf("Sensor range: cercano\n");
                    break;
                case 2:
                    printf("Sensor range: medio\n");
                    break;
                case 3:
                    printf("Sensor range: lejano\n");
                    break;
                default:
                    printf("Sensor range: fuera de rango\n");
                    break;
            }

            // "SELF-CHECKING"
            if (fabs(dist_cm - expected_dist[i]) > toleranceDistance ||
                fabs(expected_voltage[i] - calc_voltage) > toleranceVoltage ||
                sens_range != expected_range[i]) {

                SC_REPORT_ERROR("Test", "FAILED");
                all_tests_passed_val = 0;
            } else {
                printf("[INFO] Test %d PASS.\n", i + 1);
            }
            all_tests_passed.write(all_tests_passed_val);  // Escribe el valor en el sc_signal
            distance.write(dist_cm);
            voltaje.write(calc_voltage);
            distance.write(dist_cm);
            sensor.write(sens_range);

            wait(sc_time(1, SC_SEC));
            printf("\n");
        }

        if (all_tests_passed_val) {
            SC_REPORT_INFO("Test", "All the tests passed.\n");
        } else {
            SC_REPORT_ERROR("Test", "Some tests failed.\n");
        }
    }


    SC_CTOR(Testbench) {
        SC_THREAD(run_tests);
    }
};

int sc_main(int argc, char* argv[]) {
	Testbench tb("tb");

	// Para el archivo de señales "gtwave"
	sc_signal<int> all_tests_passed_signal;
	sc_signal<int> distance_signal;
	sc_signal<double> voltaje_signal;
	sc_signal<int> sensor_signal;

    tb.all_tests_passed(all_tests_passed_signal);  // Conectar la señal
    tb.distance(distance_signal);
    tb.voltaje(voltaje_signal);
    tb.sensor(sensor_signal);

	sc_trace_file *wf = sc_create_vcd_trace_file("waves");
	sc_trace(wf, all_tests_passed_signal, "tests_passed");
	sc_trace(wf, distance_signal, "distance");
	sc_trace(wf, voltaje_signal, "voltaje");
	sc_trace(wf, sensor_signal, "sensor");

	// Iniciar la simulación
	sc_start(sc_time(10, SC_SEC));  // Limitar el tiempo de simulación a 10 segundos

	// Cerrar el archivo de trazas
	sc_close_vcd_trace_file(wf);
    return 0;
}
