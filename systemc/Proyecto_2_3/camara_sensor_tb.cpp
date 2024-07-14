#include <systemc-ams.h>
#include "camara_sensor.h"
#include "camara_sensor.cpp"
#include "camara_source.h"
#include "camara_source.cpp"

int sc_main( int argc, char* argv[])
{
	using namespace sc_core;
	double const_digitalization = 190;
	//int original_width, original_height, channels;
  	//int index = 0;
  	//const char* input_filename = "_filtered.jpg";
  	//const char* output_filename = "imagen_generada.jpg";
  	//unsigned char* original_image;
	//Signals 
	sca_tdf::sca_signal<bool> cable("cable");
	//sca_tdf::sca_signal<bool> sens_active("sens_active"); 
	//sc_core::sc_signal<bool> sens_active("sens_active"); 
	sca_tdf::sca_signal<bool> image_ready_cable("digital_image_ready");	
	// Sim stop time
	const sc_time t_stop(25.0, SC_MS);
	const sc_time delay(40000.0, SC_MS);
	//unsigned char* original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 0);
		//printf("HOla2");
			
	
	
// del source
camara_source cam_source("cam_source");
cam_source.sens_active(cable);


camara_sensor cam_sensor("cam_sensor", const_digitalization);
cam_sensor.signal_in(cable);
cam_sensor.digital_image_ready(image_ready_cable);

//conexión con el módulo de Eq.


// TRACING
	sca_trace_file *tfp = sca_create_tabular_trace_file("camara_sensor_tb");
	sca_trace(tfp, cable, cable.name());
	sca_trace(tfp, image_ready_cable, image_ready_cable.name());
//	sca_trace(tfp, signal, signal.name());
	
//VCD
	sca_trace_file *sensor_vcd = sca_create_vcd_trace_file("camara_sensor_tb.vcd");
	sca_trace(sensor_vcd, cable, cable.name());
	sca_trace(sensor_vcd, image_ready_cable, image_ready_cable.name());
//sca_trace(tfp, signal, signal.name());
        cout<< "Simulation started ... " <<endl;
	//sens_active.write(false);
	//Simulation
	
	sc_start(t_stop);
	//sens_active.write(true);
	

	//close and save trace file
	sca_close_tabular_trace_file(tfp);
	
	//close and save vcd file
	sca_close_vcd_trace_file(tfp);
	//sc_start(t_stop);
	sc_stop();
	
	cout<< "Simulation finished." <<endl;
	
	return 0;
	

};
