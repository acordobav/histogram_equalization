/* CAMARA_SENSOR_H*/   //based on vibration_sensor.h

#ifndef _CAMARA_SOURCE_H
#define _CAMARA_SOURCE_H

#include <systemc-ams>

class camara_source:public sca_tdf::sca_module
{
	public: //signals
		sca_tdf:: sca_out<bool> sens_active;

		camara_source(sc_core::sc_module_name nm);
		
	void processing();
	void set_attributes();

};



#endif /* CAMARA_SENSOR_H */
