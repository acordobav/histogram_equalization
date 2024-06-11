#include "systemc.h"
#include "memory_map.h"
#include <iostream>
#include <fstream>

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
    int internal_total_animals;
    float internal_range_out;

    //Output results per hour
    float range;
    float results_1m;
    float results_2m;
    float results_3m;
    int total_animals;
    float average_out;

    //Output results per day
    float range_day;
    float results_1m_day;
    float results_2m_day;
    float results_3m_day;
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
        internal_total_animals = 0;
        internal_range_out = 0;

        range = 0;
        results_1m = 0;
        results_2m = 0;
        results_3m = 0;
        total_animals = 0;
        average_out = 0;

        range_day = 0;
        results_1m_day = 0;
        results_2m_day = 0;
        results_3m_day = 0;
        total_animals_day = 0;
        average_out_day = 0;
    }

    //------------Code Starts Here-------------------------
    void calculate_Statistics_hours(float *values,int size, bool save_data_hour){

        //Get register value ------------------------------
        // ADD CODE

        //Calculate Statistics ---------------------------
        if (save_data_hour){
            for (int i=0; i<size; i++){
                internal_total_animals += 1;
                internal_range += values[i];
            
                if (values[i] >= 2 && values[i] <= 50){
                    internal_1m += 1;
                }
                else if (values[i] > 50 && values[i] <= 200){
                    internal_2m += 1;
                }
                else if (values[i] > 200 && values[i] <= 400){
                    internal_3m += 1;
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
        total_animals = internal_total_animals;
        average_out = internal_range_out;
    }

    void calculate_Statistics_day(){

        // Fist, determine if the Statistics can be saved now
        if (count_hours >= HOURS_TO_SAVE_DAY){
            
            days_saved++;

            range_day = range;
            results_1m_day = results_1m;
            results_2m_day = results_2m;
            results_3m_day = results_3m;
            total_animals_day = total_animals;
            average_out_day = average_out;

            ofstream myfile;
            myfile.open("Statistics.txt");

            cout << myfile.is_open() << endl;
            myfile << "Corresponding day Data: " << days_saved << endl;
            myfile << "Number of hours: " << count_hours << endl;
            myfile << "Total of animals detected: " << total_animals_day << endl;
            myfile << "Total distance: " << range_day << endl;
            myfile << "Number of animals on 1st zone:" << results_1m << endl;
            myfile << "Number of animals on 2nd zone:" << results_2m << endl;
            myfile << "Number of animals on 3rd zone:" << results_3m << endl;
            myfile << "Average distance: " << average_out_day << endl;
            myfile << " " << endl;
            myfile.close();

            //Reset the hour counter
            count_hours = 0;
        }
    }
  
	void reset_Statistics(bool reset_data){
        if (reset_data){
            internal_range = 0;
            internal_1m = 0;
            internal_2m = 0;
            internal_3m = 0;
            internal_total_animals = 0;
            internal_range_out = 0;
        }
        
		//Output results ----------------------------------
        range = internal_range;
        results_1m = internal_1m;
        results_2m = internal_2m;
        results_3m = internal_3m;
        total_animals = internal_total_animals;
        average_out = internal_range_out;
    }
};
