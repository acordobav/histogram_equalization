#include "systemc.h"
#include "memory_map.h"
#include <iostream>
#include <fstream>

//necesito eliminar esto, son parte del mapa de registros
#define MEMORY_OFFSET_SAVE_DAY 0x5000 //starting memory address from which info per "day" is going to be saved
#define EN_SAVE_CURRENT_HOUR 0x4FFF //boolean variable (memory address) to indicate that the current hour can be saved

using std::ofstream;
using std::cout;
using std::endl;

SC_MODULE(estadist_unit){

    //-----------Internal variables-------------------
    int count_hours;
    int days_saved;

    //Temporal results
    float internal_range;
    float internal_1m;
    float internal_2m;
    float internal_3m;
    float internal_4m;
    int internal_total_animals;
    float internal_range_out;

    //Output results per hour
    float range;
    float results_1m;
    float results_2m;
    float results_3m;
    float results_4m;
    int total_animals;
    float average_out;

    //Output results per day
    float range_day;
    float results_1m_day;
    float results_2m_day;
    float results_3m_day;
    float results_4m_day;
    int total_animals_day;
    float average_out_day;

    // Constructor for memory
    SC_CTOR(estadist_unit){
        count_hours = 0;
        days_saved = 0;

        internal_range = 0;
        internal_1m = 0;
        internal_2m = 0;
        internal_3m = 0;
        internal_4m = 0;
        internal_total_animals = 0;
        internal_range_out = 0;

        range = 0;
        results_1m = 0;
        results_2m = 0;
        results_3m = 0;
        results_4m = 0;
        total_animals = 0;
        average_out = 0;

        range_day = 0;
        results_1m_day = 0;
        results_2m_day = 0;
        results_3m_day = 0;
        results_4m_day = 0;
        total_animals_day = 0;
        average_out_day = 0;
    }

    //------------Code Starts Here-------------------------
    void calculate_estadistics_hours(float *values,int size, bool save_data_hour){

        //Get register value ------------------------------
        // ADD CODE

        //Calculate estadistics ---------------------------
        if (save_data_hour){
            for (int i=0; i<size; i++){
                internal_total_animals += 1;
                internal_range += values[i];
            
                if (values[i] < 1){
                    internal_1m += 1;
                }
                else if (values[i] > 1 && values[i] < 2){
                    internal_2m += 1;
                }
                else if (values[i] > 2 && values[i] < 3){
                    internal_3m += 1;
                }
                else if (values[i] > 3 && values[i] < 4){
                    internal_4m += 1;
                }
                else{
                    ;
                }
            }
            internal_range_out = float(internal_range/internal_total_animals);
            count_hours++;
        }

        //Output results ----------------------------------
        range = internal_range;
        results_1m = internal_1m;
        results_2m = internal_2m;
        results_3m = internal_3m;
        results_4m = internal_4m;
        total_animals = internal_total_animals;
        average_out = internal_range_out;
    }

    void calculate_estadistics_day(){

        // Fist, determine if the estadistics can be saved now
        if (count_hours >= HOURS_TO_SAVE_DAY){
            
            days_saved++;

            range_day = range;
            results_1m_day = results_1m;
            results_2m_day = results_2m;
            results_3m_day = results_3m;
            results_4m_day = results_4m;
            total_animals_day = total_animals;
            average_out_day = average_out;

            ofstream myfile;
            myfile.open("test_1.txt");

            cout << myfile.is_open() << endl;
            myfile << "Corresponding day Data: " << days_saved << endl;
            myfile << "Number of hours: " << count_hours << endl;
            myfile << "Number of hours: " << count_hours << endl;
            myfile << "Average distance: " << range_day << endl;
            myfile << "Number of animals on 1st meter:" << results_1m << endl;
            myfile << "Number of animals on 2nd meter:" << results_2m << endl;
            myfile << "Number of animals on 3rd meter:" << results_3m << endl;
            myfile << "Number of animals on 4th meter:" << results_4m << endl;
            myfile << " " << endl;
            myfile.close();

            //Reset the hour counter
            count_hours = 0;
        }
    }
  
	void reset_estadistics(){
        internal_range = 0;
    	internal_1m = 0;
    	internal_2m = 0;
    	internal_3m = 0;
    	internal_4m = 0;
    	internal_total_animals = 0;
    	internal_range_out = 0;
      
		//Output results ----------------------------------
        range = internal_range;
        results_1m = internal_1m;
        results_2m = internal_2m;
        results_3m = internal_3m;
        results_4m = internal_4m;
        total_animals = internal_total_animals;
        average_out = internal_range_out;
    }
};
