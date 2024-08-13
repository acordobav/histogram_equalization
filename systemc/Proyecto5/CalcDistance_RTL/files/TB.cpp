// TB.cpp
#include <systemc.h>
#include <cmath>  // Para fabs
#include "CalsDis.h" // Incluye solo el encabezado


SC_MODULE(Testbench) {
    sc_signal<int> all_tests_passed;  // Cambiado a sc_signal

    void run_tests() {
        int dist_cm;
        double calc_voltage;
        int sens_range;

        const int num_measurements = 8;
        int echo = 1;

        double echo_times[] = {
            10 / 0.034, 50 / 0.034, 120 / 0.034, 180 / 0.034,
            220 / 0.034, 280 / 0.034, 350 / 0.034, 320 / 0.034
        };

        int expected_dist[] = {10, 50, 120, 180, 220, 280, 350, 320};
        double expected_voltage[] = {0.016, 0.080, 0.192, 0.288, 0.352, 0.448, 0.560, 0.512};
        int expected_range[] = {1, 1, 2, 2, 3, 3, 0, 0};

        int all_tests_passed_val = 1;
        const int toleranceDistance = 1;
        const double toleranceVoltage = 0.005;

        for (int i = 0; i < num_measurements; ++i) {
        	//sc_time wait_time(echo_times[i], SC_NS);
        	sc_time wait_time(500, SC_MS);
        	wait(wait_time);
            top(echo, echo_times[i], &dist_cm, &calc_voltage, &sens_range);

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

            if (fabs(dist_cm - expected_dist[i]) > toleranceDistance ||
                fabs(expected_voltage[i]-calc_voltage) > toleranceVoltage ||
                sens_range != expected_range[i]) {
                SC_REPORT_ERROR("Test", "fallido");
                all_tests_passed_val = 0;
            } else {
                printf("Test %d pasado.\n", i + 1);
            }
            printf("\n");
        }

        all_tests_passed.write(all_tests_passed_val);  // Escribe el valor en el sc_signal

        if (all_tests_passed_val) {
            SC_REPORT_INFO("Test", "Todos los tests pasaron correctamente.\n");
        } else {
            SC_REPORT_ERROR("Test", "Algunos tests fallaron.\n");
        }
    }

    SC_CTOR(Testbench) {
        SC_THREAD(run_tests);

    }
};

int sc_main(int argc, char* argv[]) {
    Testbench tb("tb");
    sc_trace_file *wf = sc_create_vcd_trace_file("wave");
    sc_trace(wf, tb.all_tests_passed, "all_tests_passed");
    sc_start(sc_time(20, SC_SEC));
    sc_close_vcd_trace_file(wf);
    return 0;
}
