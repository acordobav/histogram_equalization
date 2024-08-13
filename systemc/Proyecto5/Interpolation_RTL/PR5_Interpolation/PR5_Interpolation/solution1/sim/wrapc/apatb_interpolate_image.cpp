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


// wrapc file define: "input_image"
#define AUTOTB_TVIN_input_image  "../tv/cdatafile/c.interpolate_image.autotvin_input_image.dat"
// wrapc file define: "output_image"
#define AUTOTB_TVOUT_output_image  "../tv/cdatafile/c.interpolate_image.autotvout_output_image.dat"
#define AUTOTB_TVIN_output_image  "../tv/cdatafile/c.interpolate_image.autotvin_output_image.dat"

#define INTER_TCL  "../tv/cdatafile/ref.tcl"

// tvout file define: "output_image"
#define AUTOTB_TVOUT_PC_output_image  "../tv/rtldatafile/rtl.interpolate_image.autotvout_output_image.dat"

class INTER_TCL_FILE {
	public:
		INTER_TCL_FILE(const char* name) {
			mName = name;
			input_image_depth = 0;
			output_image_depth = 0;
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
			total_list << "{input_image " << input_image_depth << "}\n";
			total_list << "{output_image " << output_image_depth << "}\n";
			return total_list.str();
		}

		void set_num (int num , int* class_num) {
			(*class_num) = (*class_num) > num ? (*class_num) : num;
		}
	public:
		int input_image_depth;
		int output_image_depth;
		int trans_num;

	private:
		ofstream mFile;
		const char* mName;
};

extern void interpolate_image (
const unsigned char input_image[323][434][3],
unsigned char output_image[128][256][3]);

void AESL_WRAP_interpolate_image (
const unsigned char input_image[323][434][3],
unsigned char output_image[128][256][3])
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


		// output port post check: "output_image"
		aesl_fh.read(AUTOTB_TVOUT_PC_output_image, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_output_image, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_output_image, AESL_token); // data

			sc_bv<8> *output_image_pc_buffer = new sc_bv<8>[98304];
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
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'output_image', possible cause: There are uninitialized variables in the C design." << endl;
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
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'output_image', possible cause: There are uninitialized variables in the C design." << endl;
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
					output_image_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_output_image, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_output_image))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: output_image
				{
					// bitslice(7, 0)
					// {
						// celement: output_image(7, 0)
						// {
							sc_lv<8>* output_image_lv0_0_127_1_lv1_0_255_1_lv2_0_2_1 = new sc_lv<8>[98304];
						// }
					// }

					// bitslice(7, 0)
					{
						int hls_map_index = 0;
						// celement: output_image(7, 0)
						{
							// carray: (0) => (127) @ (1)
							for (int i_0 = 0; i_0 <= 127; i_0 += 1)
							{
								// carray: (0) => (255) @ (1)
								for (int i_1 = 0; i_1 <= 255; i_1 += 1)
								{
									// carray: (0) => (2) @ (1)
									for (int i_2 = 0; i_2 <= 2; i_2 += 1)
									{
										if (&(output_image[0][0][0]) != NULL) // check the null address if the c port is array or others
										{
											output_image_lv0_0_127_1_lv1_0_255_1_lv2_0_2_1[hls_map_index].range(7, 0) = sc_bv<8>(output_image_pc_buffer[hls_map_index].range(7, 0));
											hls_map_index++;
										}
									}
								}
							}
						}
					}

					// bitslice(7, 0)
					{
						int hls_map_index = 0;
						// celement: output_image(7, 0)
						{
							// carray: (0) => (127) @ (1)
							for (int i_0 = 0; i_0 <= 127; i_0 += 1)
							{
								// carray: (0) => (255) @ (1)
								for (int i_1 = 0; i_1 <= 255; i_1 += 1)
								{
									// carray: (0) => (2) @ (1)
									for (int i_2 = 0; i_2 <= 2; i_2 += 1)
									{
										// sub                    : i_0 i_1 i_2
										// ori_name               : output_image[i_0][i_1][i_2]
										// sub_1st_elem           : 0 0 0
										// ori_name_1st_elem      : output_image[0][0][0]
										// output_left_conversion : output_image[i_0][i_1][i_2]
										// output_type_conversion : (output_image_lv0_0_127_1_lv1_0_255_1_lv2_0_2_1[hls_map_index]).to_uint64()
										if (&(output_image[0][0][0]) != NULL) // check the null address if the c port is array or others
										{
											output_image[i_0][i_1][i_2] = (output_image_lv0_0_127_1_lv1_0_255_1_lv2_0_2_1[hls_map_index]).to_uint64();
											hls_map_index++;
										}
									}
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] output_image_pc_buffer;
		}

		AESL_transaction_pc++;
	}
	else
	{
		CodeState = ENTER_WRAPC;
		static unsigned AESL_transaction;

		static AESL_FILE_HANDLER aesl_fh;

		// "input_image"
		char* tvin_input_image = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_input_image);

		// "output_image"
		char* tvin_output_image = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_output_image);
		char* tvout_output_image = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_output_image);

		CodeState = DUMP_INPUTS;
		static INTER_TCL_FILE tcl_file(INTER_TCL);
		int leading_zero;

		// [[transaction]]
		sprintf(tvin_input_image, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_input_image, tvin_input_image);

		sc_bv<8>* input_image_tvin_wrapc_buffer = new sc_bv<8>[420546];

		// RTL Name: input_image
		{
			// bitslice(7, 0)
			{
				int hls_map_index = 0;
				// celement: input_image(7, 0)
				{
					// carray: (0) => (322) @ (1)
					for (int i_0 = 0; i_0 <= 322; i_0 += 1)
					{
						// carray: (0) => (433) @ (1)
						for (int i_1 = 0; i_1 <= 433; i_1 += 1)
						{
							// carray: (0) => (2) @ (1)
							for (int i_2 = 0; i_2 <= 2; i_2 += 1)
							{
								// sub                   : i_0 i_1 i_2
								// ori_name              : input_image[i_0][i_1][i_2]
								// sub_1st_elem          : 0 0 0
								// ori_name_1st_elem     : input_image[0][0][0]
								// regulate_c_name       : input_image
								// input_type_conversion : input_image[i_0][i_1][i_2]
								if (&(input_image[0][0][0]) != NULL) // check the null address if the c port is array or others
								{
									sc_lv<8> input_image_tmp_mem;
									input_image_tmp_mem = input_image[i_0][i_1][i_2];
									input_image_tvin_wrapc_buffer[hls_map_index].range(7, 0) = input_image_tmp_mem.range(7, 0);
                                 			       hls_map_index++;
								}
							}
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 420546; i++)
		{
			sprintf(tvin_input_image, "%s\n", (input_image_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_input_image, tvin_input_image);
		}

		tcl_file.set_num(420546, &tcl_file.input_image_depth);
		sprintf(tvin_input_image, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_input_image, tvin_input_image);

		// release memory allocation
		delete [] input_image_tvin_wrapc_buffer;

		// [[transaction]]
		sprintf(tvin_output_image, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_output_image, tvin_output_image);

		sc_bv<8>* output_image_tvin_wrapc_buffer = new sc_bv<8>[98304];

		// RTL Name: output_image
		{
			// bitslice(7, 0)
			{
				int hls_map_index = 0;
				// celement: output_image(7, 0)
				{
					// carray: (0) => (127) @ (1)
					for (int i_0 = 0; i_0 <= 127; i_0 += 1)
					{
						// carray: (0) => (255) @ (1)
						for (int i_1 = 0; i_1 <= 255; i_1 += 1)
						{
							// carray: (0) => (2) @ (1)
							for (int i_2 = 0; i_2 <= 2; i_2 += 1)
							{
								// sub                   : i_0 i_1 i_2
								// ori_name              : output_image[i_0][i_1][i_2]
								// sub_1st_elem          : 0 0 0
								// ori_name_1st_elem     : output_image[0][0][0]
								// regulate_c_name       : output_image
								// input_type_conversion : output_image[i_0][i_1][i_2]
								if (&(output_image[0][0][0]) != NULL) // check the null address if the c port is array or others
								{
									sc_lv<8> output_image_tmp_mem;
									output_image_tmp_mem = output_image[i_0][i_1][i_2];
									output_image_tvin_wrapc_buffer[hls_map_index].range(7, 0) = output_image_tmp_mem.range(7, 0);
                                 			       hls_map_index++;
								}
							}
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 98304; i++)
		{
			sprintf(tvin_output_image, "%s\n", (output_image_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_output_image, tvin_output_image);
		}

		tcl_file.set_num(98304, &tcl_file.output_image_depth);
		sprintf(tvin_output_image, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_output_image, tvin_output_image);

		// release memory allocation
		delete [] output_image_tvin_wrapc_buffer;

// [call_c_dut] ---------->

		CodeState = CALL_C_DUT;
		interpolate_image(input_image, output_image);

		CodeState = DUMP_OUTPUTS;

		// [[transaction]]
		sprintf(tvout_output_image, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_output_image, tvout_output_image);

		sc_bv<8>* output_image_tvout_wrapc_buffer = new sc_bv<8>[98304];

		// RTL Name: output_image
		{
			// bitslice(7, 0)
			{
				int hls_map_index = 0;
				// celement: output_image(7, 0)
				{
					// carray: (0) => (127) @ (1)
					for (int i_0 = 0; i_0 <= 127; i_0 += 1)
					{
						// carray: (0) => (255) @ (1)
						for (int i_1 = 0; i_1 <= 255; i_1 += 1)
						{
							// carray: (0) => (2) @ (1)
							for (int i_2 = 0; i_2 <= 2; i_2 += 1)
							{
								// sub                   : i_0 i_1 i_2
								// ori_name              : output_image[i_0][i_1][i_2]
								// sub_1st_elem          : 0 0 0
								// ori_name_1st_elem     : output_image[0][0][0]
								// regulate_c_name       : output_image
								// input_type_conversion : output_image[i_0][i_1][i_2]
								if (&(output_image[0][0][0]) != NULL) // check the null address if the c port is array or others
								{
									sc_lv<8> output_image_tmp_mem;
									output_image_tmp_mem = output_image[i_0][i_1][i_2];
									output_image_tvout_wrapc_buffer[hls_map_index].range(7, 0) = output_image_tmp_mem.range(7, 0);
                                 			       hls_map_index++;
								}
							}
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 98304; i++)
		{
			sprintf(tvout_output_image, "%s\n", (output_image_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_output_image, tvout_output_image);
		}

		tcl_file.set_num(98304, &tcl_file.output_image_depth);
		sprintf(tvout_output_image, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_output_image, tvout_output_image);

		// release memory allocation
		delete [] output_image_tvout_wrapc_buffer;

		CodeState = DELETE_CHAR_BUFFERS;
		// release memory allocation: "input_image"
		delete [] tvin_input_image;
		// release memory allocation: "output_image"
		delete [] tvout_output_image;
		delete [] tvin_output_image;

		AESL_transaction++;

		tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
	}
}

