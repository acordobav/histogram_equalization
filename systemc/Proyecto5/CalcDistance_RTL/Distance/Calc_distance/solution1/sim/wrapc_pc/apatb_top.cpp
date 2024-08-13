// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================

#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


// [dump_struct_tree [build_nameSpaceTree] dumpedStructList] ---------->


// [dump_enumeration [get_enumeration_list]] ---------->


// wrapc file define: "trigger"
#define AUTOTB_TVIN_trigger  "../tv/cdatafile/c.top.autotvin_trigger.dat"
// wrapc file define: "simulated_time"
#define AUTOTB_TVIN_simulated_time  "../tv/cdatafile/c.top.autotvin_simulated_time.dat"
// wrapc file define: "dist_cm"
#define AUTOTB_TVOUT_dist_cm  "../tv/cdatafile/c.top.autotvout_dist_cm.dat"
// wrapc file define: "calc_voltage"
#define AUTOTB_TVOUT_calc_voltage  "../tv/cdatafile/c.top.autotvout_calc_voltage.dat"
// wrapc file define: "sens_range"
#define AUTOTB_TVOUT_sens_range  "../tv/cdatafile/c.top.autotvout_sens_range.dat"

#define INTER_TCL  "../tv/cdatafile/ref.tcl"

// tvout file define: "dist_cm"
#define AUTOTB_TVOUT_PC_dist_cm  "../tv/rtldatafile/rtl.top.autotvout_dist_cm.dat"
// tvout file define: "calc_voltage"
#define AUTOTB_TVOUT_PC_calc_voltage  "../tv/rtldatafile/rtl.top.autotvout_calc_voltage.dat"
// tvout file define: "sens_range"
#define AUTOTB_TVOUT_PC_sens_range  "../tv/rtldatafile/rtl.top.autotvout_sens_range.dat"

class INTER_TCL_FILE {
	public:
		INTER_TCL_FILE(const char* name) {
			mName = name;
			trigger_depth = 0;
			simulated_time_depth = 0;
			dist_cm_depth = 0;
			calc_voltage_depth = 0;
			sens_range_depth = 0;
			trans_num =0;
		}

		~INTER_TCL_FILE() {
			mFile.open(mName);
			if (!mFile.good()) {
				cout << "Failed to open file ref.tcl" << endl;
				exit (1);
			}
			string total_list = get_depth_list();
			mFile << "set depth_list {\n";
			mFile << total_list;
			mFile << "}\n";
			mFile << "set trans_num "<<trans_num<<endl;
			mFile.close();
		}

		string get_depth_list () {
			stringstream total_list;
			total_list << "{trigger " << trigger_depth << "}\n";
			total_list << "{simulated_time " << simulated_time_depth << "}\n";
			total_list << "{dist_cm " << dist_cm_depth << "}\n";
			total_list << "{calc_voltage " << calc_voltage_depth << "}\n";
			total_list << "{sens_range " << sens_range_depth << "}\n";
			return total_list.str();
		}

		void set_num (int num , int* class_num) {
			(*class_num) = (*class_num) > num ? (*class_num) : num;
		}
	public:
		int trigger_depth;
		int simulated_time_depth;
		int dist_cm_depth;
		int calc_voltage_depth;
		int sens_range_depth;
		int trans_num;

	private:
		ofstream mFile;
		const char* mName;
};

extern void top (
int trigger,
double simulated_time,
int* dist_cm,
double* calc_voltage,
int* sens_range);

void AESL_WRAP_top (
int trigger,
double simulated_time,
int* dist_cm,
double* calc_voltage,
int* sens_range)
{
	refine_signal_handler();
	fstream wrapc_switch_file_token;
	wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
	int AESL_i;
	if (wrapc_switch_file_token.good())
	{
		CodeState = ENTER_WRAPC_PC;
		static unsigned AESL_transaction_pc = 0;
		string AESL_token;
		string AESL_num;
		static AESL_FILE_HANDLER aesl_fh;


		// output port post check: "dist_cm"
		aesl_fh.read(AUTOTB_TVOUT_PC_dist_cm, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_dist_cm, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_dist_cm, AESL_token); // data

			sc_bv<32> *dist_cm_pc_buffer = new sc_bv<32>[1];
			int i = 0;

			while (AESL_token != "[[/transaction]]")
			{
				bool no_x = false;
				bool err = false;

				// search and replace 'X' with "0" from the 1st char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('X');
					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'dist_cm', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				no_x = false;

				// search and replace 'x' with "0" from the 3rd char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('x', 2);

					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'dist_cm', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				// push token into output port buffer
				if (AESL_token != "")
				{
					dist_cm_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_dist_cm, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_dist_cm))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: dist_cm
				{
					// bitslice(31, 0)
					// {
						// celement: dist_cm(31, 0)
						// {
							sc_lv<32>* dist_cm_lv0_0_0_1 = new sc_lv<32>[1];
						// }
					// }

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: dist_cm(31, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								if (&(dist_cm[0]) != NULL) // check the null address if the c port is array or others
								{
									dist_cm_lv0_0_0_1[hls_map_index].range(31, 0) = sc_bv<32>(dist_cm_pc_buffer[hls_map_index].range(31, 0));
									hls_map_index++;
								}
							}
						}
					}

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: dist_cm(31, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								// sub                    : i_0
								// ori_name               : dist_cm[i_0]
								// sub_1st_elem           : 0
								// ori_name_1st_elem      : dist_cm[0]
								// output_left_conversion : dist_cm[i_0]
								// output_type_conversion : (dist_cm_lv0_0_0_1[hls_map_index]).to_uint64()
								if (&(dist_cm[0]) != NULL) // check the null address if the c port is array or others
								{
									dist_cm[i_0] = (dist_cm_lv0_0_0_1[hls_map_index]).to_uint64();
									hls_map_index++;
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] dist_cm_pc_buffer;
		}

		// output port post check: "calc_voltage"
		aesl_fh.read(AUTOTB_TVOUT_PC_calc_voltage, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_calc_voltage, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_calc_voltage, AESL_token); // data

			sc_bv<64> *calc_voltage_pc_buffer = new sc_bv<64>[1];
			int i = 0;

			while (AESL_token != "[[/transaction]]")
			{
				bool no_x = false;
				bool err = false;

				// search and replace 'X' with "0" from the 1st char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('X');
					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'calc_voltage', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				no_x = false;

				// search and replace 'x' with "0" from the 3rd char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('x', 2);

					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'calc_voltage', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				// push token into output port buffer
				if (AESL_token != "")
				{
					calc_voltage_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_calc_voltage, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_calc_voltage))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: calc_voltage
				{
					// bitslice(63, 0)
					// {
						// celement: calc_voltage(63, 0)
						// {
							sc_lv<64>* calc_voltage_lv0_0_0_1 = new sc_lv<64>[1];
						// }
					// }

					// bitslice(63, 0)
					{
						int hls_map_index = 0;
						// celement: calc_voltage(63, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								if (&(calc_voltage[0]) != NULL) // check the null address if the c port is array or others
								{
									calc_voltage_lv0_0_0_1[hls_map_index].range(63, 0) = sc_bv<64>(calc_voltage_pc_buffer[hls_map_index].range(63, 0));
									hls_map_index++;
								}
							}
						}
					}

					// bitslice(63, 0)
					{
						int hls_map_index = 0;
						// celement: calc_voltage(63, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								// sub                    : i_0
								// ori_name               : calc_voltage[i_0]
								// sub_1st_elem           : 0
								// ori_name_1st_elem      : calc_voltage[0]
								// output_left_conversion : *(long long*)&calc_voltage[i_0]
								// output_type_conversion : (calc_voltage_lv0_0_0_1[hls_map_index]).to_uint64()
								if (&(calc_voltage[0]) != NULL) // check the null address if the c port is array or others
								{
									*(long long*)&calc_voltage[i_0] = (calc_voltage_lv0_0_0_1[hls_map_index]).to_uint64();
									hls_map_index++;
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] calc_voltage_pc_buffer;
		}

		// output port post check: "sens_range"
		aesl_fh.read(AUTOTB_TVOUT_PC_sens_range, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_sens_range, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_sens_range, AESL_token); // data

			sc_bv<32> *sens_range_pc_buffer = new sc_bv<32>[1];
			int i = 0;

			while (AESL_token != "[[/transaction]]")
			{
				bool no_x = false;
				bool err = false;

				// search and replace 'X' with "0" from the 1st char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('X');
					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'sens_range', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				no_x = false;

				// search and replace 'x' with "0" from the 3rd char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('x', 2);

					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'sens_range', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				// push token into output port buffer
				if (AESL_token != "")
				{
					sens_range_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_sens_range, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_sens_range))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: sens_range
				{
					// bitslice(31, 0)
					// {
						// celement: sens_range(31, 0)
						// {
							sc_lv<32>* sens_range_lv0_0_0_1 = new sc_lv<32>[1];
						// }
					// }

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: sens_range(31, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								if (&(sens_range[0]) != NULL) // check the null address if the c port is array or others
								{
									sens_range_lv0_0_0_1[hls_map_index].range(31, 0) = sc_bv<32>(sens_range_pc_buffer[hls_map_index].range(31, 0));
									hls_map_index++;
								}
							}
						}
					}

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: sens_range(31, 0)
						{
							// carray: (0) => (0) @ (1)
							for (int i_0 = 0; i_0 <= 0; i_0 += 1)
							{
								// sub                    : i_0
								// ori_name               : sens_range[i_0]
								// sub_1st_elem           : 0
								// ori_name_1st_elem      : sens_range[0]
								// output_left_conversion : sens_range[i_0]
								// output_type_conversion : (sens_range_lv0_0_0_1[hls_map_index]).to_uint64()
								if (&(sens_range[0]) != NULL) // check the null address if the c port is array or others
								{
									sens_range[i_0] = (sens_range_lv0_0_0_1[hls_map_index]).to_uint64();
									hls_map_index++;
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] sens_range_pc_buffer;
		}

		AESL_transaction_pc++;
	}
	else
	{
		CodeState = ENTER_WRAPC;
		static unsigned AESL_transaction;

		static AESL_FILE_HANDLER aesl_fh;

		// "trigger"
		char* tvin_trigger = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_trigger);

		// "simulated_time"
		char* tvin_simulated_time = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_simulated_time);

		// "dist_cm"
		char* tvout_dist_cm = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_dist_cm);

		// "calc_voltage"
		char* tvout_calc_voltage = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_calc_voltage);

		// "sens_range"
		char* tvout_sens_range = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_sens_range);

		CodeState = DUMP_INPUTS;
		static INTER_TCL_FILE tcl_file(INTER_TCL);
		int leading_zero;

		// [[transaction]]
		sprintf(tvin_trigger, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_trigger, tvin_trigger);

		sc_bv<32> trigger_tvin_wrapc_buffer;

		// RTL Name: trigger
		{
			// bitslice(31, 0)
			{
				// celement: trigger(31, 0)
				{
					// carray: (0) => (0) @ (0)
					{
						// sub                   : 
						// ori_name              : trigger
						// sub_1st_elem          : 
						// ori_name_1st_elem     : trigger
						// regulate_c_name       : trigger
						// input_type_conversion : trigger
						if (&(trigger) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> trigger_tmp_mem;
							trigger_tmp_mem = trigger;
							trigger_tvin_wrapc_buffer.range(31, 0) = trigger_tmp_mem.range(31, 0);
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1; i++)
		{
			sprintf(tvin_trigger, "%s\n", (trigger_tvin_wrapc_buffer).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_trigger, tvin_trigger);
		}

		tcl_file.set_num(1, &tcl_file.trigger_depth);
		sprintf(tvin_trigger, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_trigger, tvin_trigger);

		// [[transaction]]
		sprintf(tvin_simulated_time, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_simulated_time, tvin_simulated_time);

		sc_bv<64> simulated_time_tvin_wrapc_buffer;

		// RTL Name: simulated_time
		{
			// bitslice(63, 0)
			{
				// celement: simulated_time(63, 0)
				{
					// carray: (0) => (0) @ (0)
					{
						// sub                   : 
						// ori_name              : simulated_time
						// sub_1st_elem          : 
						// ori_name_1st_elem     : simulated_time
						// regulate_c_name       : simulated_time
						// input_type_conversion : *(long long*)&simulated_time
						if (&(simulated_time) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<64> simulated_time_tmp_mem;
							simulated_time_tmp_mem = *(long long*)&simulated_time;
							simulated_time_tvin_wrapc_buffer.range(63, 0) = simulated_time_tmp_mem.range(63, 0);
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1; i++)
		{
			sprintf(tvin_simulated_time, "%s\n", (simulated_time_tvin_wrapc_buffer).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_simulated_time, tvin_simulated_time);
		}

		tcl_file.set_num(1, &tcl_file.simulated_time_depth);
		sprintf(tvin_simulated_time, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_simulated_time, tvin_simulated_time);

// [call_c_dut] ---------->

		CodeState = CALL_C_DUT;
		top(trigger, simulated_time, dist_cm, calc_voltage, sens_range);

		CodeState = DUMP_OUTPUTS;

		// [[transaction]]
		sprintf(tvout_dist_cm, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_dist_cm, tvout_dist_cm);

		sc_bv<32>* dist_cm_tvout_wrapc_buffer = new sc_bv<32>[1];

		// RTL Name: dist_cm
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: dist_cm(31, 0)
				{
					// carray: (0) => (0) @ (1)
					for (int i_0 = 0; i_0 <= 0; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : dist_cm[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : dist_cm[0]
						// regulate_c_name       : dist_cm
						// input_type_conversion : dist_cm[i_0]
						if (&(dist_cm[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> dist_cm_tmp_mem;
							dist_cm_tmp_mem = dist_cm[i_0];
							dist_cm_tvout_wrapc_buffer[hls_map_index].range(31, 0) = dist_cm_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1; i++)
		{
			sprintf(tvout_dist_cm, "%s\n", (dist_cm_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_dist_cm, tvout_dist_cm);
		}

		tcl_file.set_num(1, &tcl_file.dist_cm_depth);
		sprintf(tvout_dist_cm, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_dist_cm, tvout_dist_cm);

		// release memory allocation
		delete [] dist_cm_tvout_wrapc_buffer;

		// [[transaction]]
		sprintf(tvout_calc_voltage, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_calc_voltage, tvout_calc_voltage);

		sc_bv<64>* calc_voltage_tvout_wrapc_buffer = new sc_bv<64>[1];

		// RTL Name: calc_voltage
		{
			// bitslice(63, 0)
			{
				int hls_map_index = 0;
				// celement: calc_voltage(63, 0)
				{
					// carray: (0) => (0) @ (1)
					for (int i_0 = 0; i_0 <= 0; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : calc_voltage[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : calc_voltage[0]
						// regulate_c_name       : calc_voltage
						// input_type_conversion : *(long long*)&calc_voltage[i_0]
						if (&(calc_voltage[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<64> calc_voltage_tmp_mem;
							calc_voltage_tmp_mem = *(long long*)&calc_voltage[i_0];
							calc_voltage_tvout_wrapc_buffer[hls_map_index].range(63, 0) = calc_voltage_tmp_mem.range(63, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1; i++)
		{
			sprintf(tvout_calc_voltage, "%s\n", (calc_voltage_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_calc_voltage, tvout_calc_voltage);
		}

		tcl_file.set_num(1, &tcl_file.calc_voltage_depth);
		sprintf(tvout_calc_voltage, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_calc_voltage, tvout_calc_voltage);

		// release memory allocation
		delete [] calc_voltage_tvout_wrapc_buffer;

		// [[transaction]]
		sprintf(tvout_sens_range, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_sens_range, tvout_sens_range);

		sc_bv<32>* sens_range_tvout_wrapc_buffer = new sc_bv<32>[1];

		// RTL Name: sens_range
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: sens_range(31, 0)
				{
					// carray: (0) => (0) @ (1)
					for (int i_0 = 0; i_0 <= 0; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : sens_range[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : sens_range[0]
						// regulate_c_name       : sens_range
						// input_type_conversion : sens_range[i_0]
						if (&(sens_range[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> sens_range_tmp_mem;
							sens_range_tmp_mem = sens_range[i_0];
							sens_range_tvout_wrapc_buffer[hls_map_index].range(31, 0) = sens_range_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1; i++)
		{
			sprintf(tvout_sens_range, "%s\n", (sens_range_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_sens_range, tvout_sens_range);
		}

		tcl_file.set_num(1, &tcl_file.sens_range_depth);
		sprintf(tvout_sens_range, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_sens_range, tvout_sens_range);

		// release memory allocation
		delete [] sens_range_tvout_wrapc_buffer;

		CodeState = DELETE_CHAR_BUFFERS;
		// release memory allocation: "trigger"
		delete [] tvin_trigger;
		// release memory allocation: "simulated_time"
		delete [] tvin_simulated_time;
		// release memory allocation: "dist_cm"
		delete [] tvout_dist_cm;
		// release memory allocation: "calc_voltage"
		delete [] tvout_calc_voltage;
		// release memory allocation: "sens_range"
		delete [] tvout_sens_range;

		AESL_transaction++;

		tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
	}
}

