#include <systemc-ams.h>
#include "camara_sensor.h"
#include "camara_sensor.cpp"
//#include "camara_source.h"
//#include "camara_source.cpp"
#include "dist_calc.cpp"
#include <systemc.h>

//#include "dist_calc.cpp"

bool trigger_check(bool act){
    if(act == true){
      sc_start(200,SC_MS);
	  return true;
    }
    else{
      sc_start(5,SC_US);
      return false;
    }
}
  
int sc_main( int argc, char* argv[])
{
	using namespace sc_core;
	double const_digitalization = 190;
	double tiempo; 
 	/*sca_tdf::*/sc_signal<sc_int <32> > dist_cm; 
  	sc_signal<double > calc_voltage;
  	//sc_signal<bool> sens_active;
  	sca_tdf::sca_signal<bool> sens_active; //THIS 
  	sc_signal<const char*> sens_range;
  	sc_signal<bool > trigger;
  	sc_signal<bool > echo;
  	sc_signal<sc_int <32> > count_near;
  	sc_signal<sc_int <32> > count_half;
  	sc_signal<sc_int <32> > count_far;
  	bool tmp;
  	bool echo_temp;
  	int time;
  
	/*sca_tdf::*/sc_signal<bool> sens_active_cable("sens_active_cable");
	
	//sca_tdf::sca_signal<bool> sens_active("sens_active"); 
	//sc_core::sc_signal<bool> sens_active("sens_active"); 
	sca_tdf::sca_signal<bool> image_ready_cable("image_ready_cable");	
	// Sim stop time
	const sc_time t_stop(25.0, SC_MS);
	const sc_time delay(40000.0, SC_MS);
	//unsigned char* original_image = stbi_load(input_filename, &original_width, &original_height, &channels, 0);
		//printf("HOla2");
			
	
dist_calc dist ("DIST");
	dist.dist_cm(dist_cm);
	dist.calc_voltage(calc_voltage);
	dist.sens_range(sens_range);
	dist.trigger(trigger);
 	dist.echo(echo);
	dist.count_near(count_near);
 	dist.count_half(count_half);
  	dist.count_far(count_far);
  	dist.sens_active(sens_active_cable); // from dist.calc
// del source
//camara_source cam_source("cam_source");
//cam_source.sens_active(cable);


//dist_calc distance("sens_active");
//distance.sens_active(sens_active_cable);


camara_sensor cam_sensor("cam_sensor", const_digitalization, tiempo);
cam_sensor.signal_in(sens_active_cable);
cam_sensor.digital_image_ready(image_ready_cable);

 

//conexión con el módulo de Eq.


// TRACING
//	sca_trace_file *tfp = sca_create_tabular_trace_file("camara_sensor_tb");
//	sca_trace(tfp, cable, cable.name());
//	sca_trace(tfp, image_ready_cable, image_ready_cable.name());
//	sca_trace(tfp, signal, signal.name());
	
//VCD
//	sca_trace_file *sensor_vcd = sca_create_vcd_trace_file("camara_sensor_tb.vcd");
//	sca_trace(sensor_vcd, cable, cable.name());
//	sca_trace(sensor_vcd, image_ready_cable, image_ready_cable.name());
//sca_trace(tfp, signal, signal.name());
	
        cout<< "Simulation started ... " <<endl;
	//sens_active.write(false);
	//Simulation
	 sc_start(20,SC_US);
	dist.delay_start();  
	while(true) {
	
    	tmp = trigger.read();
    	echo_temp = trigger_check(tmp);
    	echo.write(echo_temp); // echo enabled
    	if(echo_temp == true) {
		
      		time = 30 + (rand() % (15000 - 30 +1) );  //randomize (30 - 15000) time elapsed according to distance
      		sc_start(time,SC_US); // wait that time to represent distance t2 - t1
      		dist.e_wait();
    	}
    	if(sc_time_stamp().to_seconds() >= 10) {
      		break;
    	}
  	}
	sc_start(t_stop);
	//sens_active.write(true);
	

	//close and save trace file
//	sca_close_tabular_trace_file(tfp);
	
	//close and save vcd file
//	sca_close_vcd_trace_file(tfp);
	//sc_start(t_stop);
	sc_stop();
	
	cout<< "Simulation finished." <<endl;
	
	return 0; 
	

};
