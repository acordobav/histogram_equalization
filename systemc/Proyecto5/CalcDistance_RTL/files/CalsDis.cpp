// CalsDis.cpp
#include "CalsDis.h"
void top(int echo, double simulated_time, int *dist_cm, double *calc_voltage, int *sens_range) {
	*dist_cm = 0;
	*calc_voltage = 0.0;
	*sens_range = 0;

	if (echo == 1) {
		*dist_cm = (int)(simulated_time * 0.034);
		*calc_voltage = *dist_cm * 0.0016;

		if (*dist_cm >= 4 && *dist_cm <= 100) {
			*sens_range = 1; // cercano
		} else if (*dist_cm > 100 && *dist_cm <= 200) {
			*sens_range = 2; // medio
		} else if (*dist_cm > 200 && *dist_cm <= 300) {
			*sens_range = 3; // lejano
		} else {
			*sens_range = 0; // fuera_de_rango
		}
	}
}

