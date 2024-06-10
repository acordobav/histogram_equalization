// Code your testbench here.
// Uncomment the next line for SystemC modules.


#include <systemc.h>
//#include <scv.h>
#include "dist_calc.cpp"
 
bool trigger_check(bool act){
    //cout << "trigger tb = " << trigger.read() <<endl;
    if(act == true){
      //printf("- - -200us - - - \n");
      sc_start(200,SC_MS);// sc_start(200,SC_US);
      return true;
    }
    else{
      sc_start(5,SC_US);
      return false;
    }
  }

int sc_main(int argc, char* argv[]) {
              
  sc_signal<double> senal_s;
  sc_signal<sc_int <32> > dist_cm; 
  sc_signal<bool> sens_active;
  sc_signal<const char*> sens_range;
  sc_signal<bool > trigger;
  sc_signal<bool > echo;
  sc_signal<sc_int <32> > count_near;
  sc_signal<sc_int <32> > count_half;
  sc_signal<sc_int <32> > count_far;
  
  
  //sc_signal<bool > ;
  
  bool tmp;
  bool echo_temp;
  int time;
  
  
  //int seed[] = {60,40,80};
  
  
  dist_calc dist ("DIST");
  	dist.senal_s(senal_s);
    dist.dist_cm(dist_cm);
    dist.sens_active(sens_active);
  	dist.sens_range(sens_range);
  	dist.trigger(trigger);
  	dist.echo(echo);
  	dist.count_near(count_near);
  	dist.count_half(count_half);
    dist.count_far(count_far);
  
  	//dist.();
	
  
  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file("dist");
  wf->set_time_unit(1, SC_NS);
  //dist.control_register.range
  
  // Dump the desired signals
  sc_trace(wf, dist_cm, "dist_cm");
  sc_trace(wf, sens_active, "sens_active");
  sc_trace(wf, trigger, "trigger");
  sc_trace(wf, echo, "echo");
  sc_trace(wf, count_near, "count_near");
  sc_trace(wf, count_half, "count_half");
  sc_trace(wf, count_far, "count_far");
  
  
  
  sc_start();
  cout << "@" << sc_time_stamp()<< endl;   
  printf("Writing in zero time\n");
  
  printf("- - -PROBANDO DISTANCIA - - - \n");
  sc_start(20,SC_US);

  dist.delay_start();  
  sc_start(40,SC_US);
  while(true) {
    tmp = trigger.read();
    //cout << "tmp = " << tmp <<endl;
    echo_temp = trigger_check(tmp);
    echo.write(echo_temp); // echo enabled?
    if(echo_temp == true) {
      time = 30 + (rand() % (15000 - 30 +1) );  //randomize (30 - 15000) time elapsed according to distance
      sc_start(time,SC_US); // wait that time to represent distance t2 - t1
    }
    if(sc_time_stamp().to_seconds() >= 10) {    // simulation time max= 10s
      break;
    }
  }
  //sc_start(40,SC_US);
  //printf("- - -PROBANDO DISTANCIA 2 - - - \n");
  
  //sc_start(60,SC_US);
  //printf("- - -PROBANDO DISTANCIA 4 - - - \n");
//  trigger_check(tmp, );
  //sc_start(200,SC_US);
  //cout << "dist_cm tb = " << dist_cm.read() <<endl;
  //cout << "sens_range tb = " << sens_range.read() <<endl;
  //cout << "sens_active tb = " << sens_active.read() <<endl;
  //cout << "count_near tb = " << count_near.read() <<endl;
  //cout << "count_half tb = " << count_half.read() <<endl;
  //cout << "count_far tb = " << count_far.read() <<endl;
  
  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
  sc_close_vcd_trace_file(wf);
  return 0;// Terminate simulation

 }



 
