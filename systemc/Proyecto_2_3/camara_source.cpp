#include <systemc.h>
#include "camara_source.h"


camara_source::camara_source(sc_core::sc_module_name nm)
:
//sens_active
//digital_image_ready
sens_active("sens_active")
{}

void camara_source::set_attributes()
{
	sens_active.set_timestep(100000000.0, sc_core::SC_MS);
}
void camara_source::processing()
{
	bool prueba = false; 
	//wait(10,SC_MS );
	prueba = true; 
	sens_active.write(prueba);
}




