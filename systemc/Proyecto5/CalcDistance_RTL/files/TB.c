// testbench.c

#include <stdio.h>
#include <math.h>  // para el uso de la función pow

// Declaración de la función top
void top(int echo, double simulated_time_microseconds, int *dist_cm, double *calc_voltage, int *sens_range);

// Función para convertir distancia en tiempo de eco (microsegundos)
int distance_to_echo_time(double distance_cm) {
    return (int)(distance_cm / 0.034);
}

// Función principal de prueba
int main() {
    int dist_cm;
    double calc_voltage;
    int sens_range;

    const int num_measurements = 8;
    int echo = 1;

    // Tiempos de eco en microsegundos para diferentes distancias
    double echo_times[] = {
        distance_to_echo_time(10),  // cercano
        distance_to_echo_time(50),  // cercano
        distance_to_echo_time(120), // medio
        distance_to_echo_time(180), // medio
        distance_to_echo_time(220), // lejano
        distance_to_echo_time(280), // lejano
        distance_to_echo_time(350), // fuera de rango
        distance_to_echo_time(320)  // fuera de rango
    };

    // Valores esperados para cada medición
    int expected_dist[] = {10, 50, 120, 180, 220, 280, 350, 320};
    double expected_voltage[] = {0.016, 0.080, 0.192, 0.288, 0.352, 0.448, 0.560, 0.512};
    int expected_range[] = {1, 1, 2, 2, 3, 3, 0, 0};

    int all_tests_passed = 1;
    const int toleranceDistance = 1;
    const double toleranceVoltage = 0.005;

    for (int i = 0; i < num_measurements; ++i) {
        printf("Medición número: %d\n", i + 1);

        // Llamada a la función top
        top(echo, echo_times[i], &dist_cm, &calc_voltage, &sens_range);

        // Mostrar resultados
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

        // Self-checking
        if (fabs(dist_cm - expected_dist[i]) > toleranceDistance ||
            fabs( expected_voltage[i] - calc_voltage ) > toleranceVoltage ||
			sens_range != expected_range[i]) {
            printf("Test %d fallido.\n", i + 1);
            all_tests_passed = 0;
        } else {
            printf("Test %d pasado.\n", i + 1);
        }

        printf("\n");
    }

    if (all_tests_passed==1) {
        printf("Todos los tests pasaron correctamente.\n");
    } else {
        printf("Algunos tests fallaron.\n");
    }

    return 0;
}
