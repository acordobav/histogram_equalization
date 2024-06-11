// Code your design here.
// Uncomment the next line for SystemC modules.
// #include <systemc>

 //-----------------------------------------------------
#include "systemc.h"


SC_MODULE (dist_calc) {
  sc_in<double> senal_s;
  sc_out<sc_int <32> > dist_cm;
  sc_out<bool > sens_active;
  sc_out<const char*> sens_range;
  sc_out<bool > trigger;
  sc_in<bool > echo;
  sc_out<sc_int <32> > count_near;
  sc_out<sc_int <32> > count_half;
  sc_out<sc_int <32> > count_far;
  
    
  //-----------Internal variables-------------------
  sc_event d_capture, d_start, end_wait;
  sc_int<64> resultado, t1, t2;
  sc_lv<32> control_register;
  sc_int <32> range1 = 2;     
  sc_int <32> range2 = 50;   
  sc_int <32> range3 = 200;  
  sc_int <32> range4 = 400;  
  sc_int <32> cuenta_near = 0;
  sc_int <32> cuenta_half = 0;
  sc_int <32> cuenta_far = 0;
  

  // Constructor 
  SC_CTOR(dist_calc) {
   SC_THREAD(pulse_trigger);
   SC_THREAD(pulse_echo);
   SC_THREAD(end_wait_th);
  } 

  //------------Code Starts Here-------------------------
  
  void delay_start(){
  	d_start.notify(2, SC_US);
  }

   void e_wait(){
  	end_wait.notify(0, SC_US);
  }
  
  
  void pulse_trigger(){ 
    wait(d_start);
	while(true) {
      	trigger=1;
        wait(10, SC_US);
        trigger=0;
        wait(1000, SC_MS);  // each 1s trigger signal will be raised
      
      }  
	}
  
  void end_wait_th(){ 
    wait(d_start);
	while(true) {
      if (echo.read() == 0){
      	end_wait.notify(0, SC_US);
      }
      wait(1, SC_US);
    }  
  };
  
  void pulse_echo(){ 
    wait(d_start);
	while(true) {
      if (echo.read() == 1){
      	t1=sc_time_stamp().to_seconds()*1000000;
        wait(end_wait);
      	t2=sc_time_stamp().to_seconds()*1000000;
        dist_cm = (t2-t1)*0.034;
      	sensor_range();
      };
      wait(50, SC_US);
    }  
  }
  
  
  void sensor_range(){
    	wait(30, SC_US);
    	if ((dist_cm)>=range1 && (dist_cm)<=range2){
            sens_range = "cercano";
    		sens_active = 1;
          	wait(300, SC_US);
            sens_active = 0;
			cuenta_near = cuenta_near+1;
          	count_near = cuenta_near;
    	}
    	else if ((dist_cm)>range2 && (dist_cm)<=range3){
    		sens_range = "medio";
          	sens_active = 1;
			wait(300, SC_US);
            sens_active = 0;
			cuenta_half = cuenta_half+1;
			count_half = cuenta_half;
    	}
    	else if ((dist_cm)>range3 && (dist_cm)<=range4){
    		sens_range = "lejano";
          	sens_active = 1;
          	wait(300, SC_US);
            sens_active = 0;
            cuenta_far = cuenta_far+1;
            count_far = cuenta_far;     
    	}
    	else{	
          	sens_range = "fuera_de_rango";
    		sens_active = 0;
        }
  }

}; 
