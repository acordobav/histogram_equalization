#include <systemc.h>
#include "camara_sensor.h"
#include <systemc-ams.h>

#define ROWS 323
#define COLS 434

int timer = 0;
int count = 0;
unsigned int prueba[420546];
unsigned int analog_prueba[420546];
unsigned int digital_prueba[420546];
uint8_t save_image[ROWS][COLS][3] = {0};

camara_sensor::camara_sensor(sc_core::sc_module_name nm, double const_digitalization_, double tiempo_)
	: sens_active_in("sens_active_in")
	, digital_image_ready("digital_image_ready")
	, const_digitalization(const_digitalization_)
{
}

void camara_sensor::set_attributes() {
	accept_attribute_changes();

	does_attribute_changes();

	set_timestep(1.0, sc_core::SC_MS);
}

void camara_sensor::change_attributes() {
	// request_next_activation(signal_in.default_event());
}

void camara_sensor::processing()
{
	int original_width, original_height, channels;
	int index = 0;
	unsigned char *original_image;
	if (timer == 500000) {
		sc_stop();
	}
	else {
		timer++;
	}

	if (sens_active_in.read()) {
		digital_image_ready.write(true);
	}
}
