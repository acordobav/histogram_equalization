#include <systemc.h>
#include "estadisticas_pv.cpp"


int sc_main (int argc, char* argv[]) {
              
    float op_range;
    float op_results_1m;
    float op_results_2m;
    float op_results_3m;
    int op_total_animals;
    float op_range_out;

    float values[12]={2, 60, 300, 
                       2, 60, 300, 
                       2, 60, 300, 
                       2, 60, 300};
  
    float values2[20]={10, 100, 300, 2, 50, 
                       10, 100, 300, 2, 50, 
                       10, 100, 300, 2, 50, 
                       10, 100, 300, 2, 50};

    float values3[4]={10, 
                       10, 
                       10, 
                       10};
  
    estadist_unit estadist ("estadist");


    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("estadist");
    wf->set_time_unit(1, SC_NS);

    //------------------------------------------------

    // Dump the desired signals
    sc_trace(wf, op_range, "op_range");
    sc_trace(wf, op_results_1m, "op_results_1m");
    sc_trace(wf, op_results_2m, "op_results_2m");
    sc_trace(wf, op_results_3m, "op_results_3m");
    sc_trace(wf, op_total_animals, "op_total_animals");
    sc_trace(wf, op_range_out, "op_range_out");

    sc_start();
    cout << "@" << sc_time_stamp()<< endl;

    printf("Writing in zero time\n");

    //TB starting here ------------------------------------

    //TEST 1 ----------------------------------------------

    printf("Writing next iteration\n");
    
    bool save_data_hour = false;

    estadist.calculate_Statistics_hours(values, 12, save_data_hour);

    sc_start(10,SC_NS);
    cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
    cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
    cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
    cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
    cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
    cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;  
    cout << "@" << sc_time_stamp()<< endl;

    //TEST 2 ----------------------------------------------

    printf("Writing next iteration\n");

    save_data_hour = true;

    estadist.calculate_Statistics_hours(values, 12, save_data_hour);

    sc_start(10,SC_NS);
    cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
    cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
    cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
    cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
    cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
    cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;  
    cout << "@" << sc_time_stamp()<< endl;

    //TEST 3 ----------------------------------------------

    printf("Writing next iteration\n");

    estadist.calculate_Statistics_hours(values2, 20, save_data_hour);

    sc_start(210,SC_NS);
    cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
    cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
    cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
    cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
    cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
    cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;

    //TEST 4 ----------------------------------------------
    
    for (int i=0; i<30; i++){
        estadist.calculate_Statistics_hours(values3, 4, save_data_hour);
        estadist.calculate_Statistics_day();

        cout << "Iteration: " << i << endl;
        cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
        cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
        cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
        cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
        cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
        cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;

        cout << "DAYS"<< endl;
        cout << "@" << sc_time_stamp() << " Range: " << estadist.range_day << endl;
        cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m_day << endl;
        cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m_day << endl;
        cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m_day << endl;
        cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals_day << endl;
        cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out_day << endl;
    }
    
    
    //TEST 5 ----------------------------------------------

    printf("Writing next iteration\n");

    bool reset_data = false; //register in charge to enable the reset of the data

    estadist.reset_Statistics(reset_data);
  
    sc_start(20,SC_NS);
    cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
    cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
    cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
    cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
    cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
    cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;


    //TEST 6 ----------------------------------------------

    printf("Writing next iteration\n");

    reset_data = true; //register in charge to enable the reset of the data

    estadist.reset_Statistics(reset_data);
  
    sc_start(20,SC_NS);
    cout << "@" << sc_time_stamp() << " Range: " << estadist.range << endl;
    cout << "@" << sc_time_stamp() << " Results 1st zone: " << estadist.results_1m << endl;
    cout << "@" << sc_time_stamp() << " Results 2nd zone: " << estadist.results_2m << endl;
    cout << "@" << sc_time_stamp() << " Results 3rd zone: " << estadist.results_3m << endl;
    cout << "@" << sc_time_stamp() << " Results total animal: " << estadist.total_animals << endl;
    cout << "@" << sc_time_stamp() << " Results average distance: " << estadist.average_out << endl;
  
    cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
    return 0;// Terminate simulation

 }
