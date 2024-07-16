#include <systemc-ams.h>
#include "camara_sensor.h"
#include "camara_sensor.cpp"
//#include "camara_source.h"
//#include "camara_source.cpp"
#include "dist_calc.cpp"
#include <systemc.h>

//#include "dist_calc.cpp"

  
int sc_main( int argc, char* argv[])
{
	using namespace sc_core;
	double const_digitalization = 190;
	double tiempo; 
 	/*sca_tdf::*/sc_signal<sc_int <32> > dist_cm; 
  	sc_signal<double > calc_voltage;
  	double echo_value = 15.00 ; //victor
  	//sc_signal<bool> sens_active;
  	sca_tdf::sca_signal<bool> sens_active; //THIS 
  	sc_signal<double> echo_signal; // victor
  	sc_signal<double> time_signal; //victor
  	sc_signal<const char*> sens_range;
  	sc_signal<bool > trigger;
  	//sc_signal<bool > echo;
  	sc_signal<sc_int <32> > count_near;
  	sc_signal<sc_int <32> > count_half;
  	sc_signal<sc_int <32> > count_far;
  	double time_signal_value=15.00;

  	bool echo_temp;
  	int time;
  
	/*sca_tdf::*/sc_signal<bool> sens_active_cable("sens_active_cable");
	
	sc_signal<double> echo_signal_cable("echo_signal_cable");
	sc_signal<double> time_signal_cable("time_signal_cable");
	sca_tdf::sca_signal<bool> image_ready_cable("image_ready_cable");	
	// Sim stop time
	const sc_time t_stop(2.0, SC_MS);
	const sc_time delay(40000.0, SC_MS);

			
	
dist_calc dist ("DIST");
	dist.dist_cm(dist_cm);
	dist.calc_voltage(calc_voltage);
	dist.sens_range(sens_range);
	dist.trigger(trigger);
 	dist.echo_signal(echo_signal_cable);
 	dist.time_signal(time_signal_cable);
	dist.count_near(count_near);
 	dist.count_half(count_half);
  	dist.count_far(count_far);
  	dist.sens_active(sens_active_cable); // from dist.calc
 

camara_sensor cam_sensor("cam_sensor", const_digitalization, tiempo);
cam_sensor.signal_in(sens_active_cable);
cam_sensor.digital_image_ready(image_ready_cable);

	
        cout<< "Simulation started ... " <<endl;

	sc_start(20,SC_US);
	dist.delay_start();  
	while(true) {
		//cout<< "Inside the while... " <<endl;
    		if(echo_value != 0) {
    			sc_start(20,SC_US);
			echo_signal.write(echo_value);
			time_signal.write(time_signal_value);
      			dist.e_wait();
    		}
    	if(sc_time_stamp().to_seconds() >= 5) {
      		break;
    	}
  	}
	sc_start(t_stop);

	sc_stop();
	
	cout<< "Simulation finished." <<endl;
	
	return 0; 
	

};
