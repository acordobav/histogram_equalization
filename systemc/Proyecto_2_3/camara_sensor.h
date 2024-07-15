/* CAMARA_SENSOR_H*/   //based on vibration_sensor.h

#ifndef _CAMARA_SENSOR_H
#define _CAMARA_SENSOR_H

#define ROWS 323
#define COLS 434
#include <systemc-ams>

class camara_sensor : public sca_tdf::sca_module
{
	public: //signals
		sca_tdf:: sc_in<bool> signal_in; 
		sca_tdf:: sca_out<bool> digital_image_ready;

		camara_sensor(sc_core::sc_module_name nm, double const_digitalization, double tiempo);
		
	void processing();
	
	void set_attributes();

	private: 
		double const_digitalization;
		double tiempo; 
		/*unsigned int prueba[420546];
		unsigned int analog_prueba[420546];
		unsigned int digital_prueba[420546]; 
		uint8_t save_image[ROWS][COLS][3] = {0};*/


};



#endif /* CAMARA_SENSOR_H */
