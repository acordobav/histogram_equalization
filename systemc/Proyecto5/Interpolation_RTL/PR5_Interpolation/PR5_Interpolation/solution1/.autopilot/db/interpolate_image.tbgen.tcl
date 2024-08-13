set moduleName interpolate_image
set isTopModule 1
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {interpolate_image}
set C_modelType { void 0 }
set C_modelArgList {
	{ input_image int 8 regular {array 420546 { 1 1 } 1 1 }  }
	{ output_image int 8 regular {array 98304 { 0 3 } 0 1 }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "input_image", "interface" : "memory", "bitwidth" : 8, "direction" : "READONLY", "bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "input_image","cData": "unsigned char","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 322,"step" : 1},{"low" : 0,"up" : 433,"step" : 1},{"low" : 0,"up" : 2,"step" : 1}]}]}]} , 
 	{ "Name" : "output_image", "interface" : "memory", "bitwidth" : 8, "direction" : "WRITEONLY", "bitSlice":[{"low":0,"up":7,"cElement": [{"cName": "output_image","cData": "unsigned char","bit_use": { "low": 0,"up": 7},"cArray": [{"low" : 0,"up" : 127,"step" : 1},{"low" : 0,"up" : 255,"step" : 1},{"low" : 0,"up" : 2,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 16
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ input_image_address0 sc_out sc_lv 19 signal 0 } 
	{ input_image_ce0 sc_out sc_logic 1 signal 0 } 
	{ input_image_q0 sc_in sc_lv 8 signal 0 } 
	{ input_image_address1 sc_out sc_lv 19 signal 0 } 
	{ input_image_ce1 sc_out sc_logic 1 signal 0 } 
	{ input_image_q1 sc_in sc_lv 8 signal 0 } 
	{ output_image_address0 sc_out sc_lv 17 signal 1 } 
	{ output_image_ce0 sc_out sc_logic 1 signal 1 } 
	{ output_image_we0 sc_out sc_logic 1 signal 1 } 
	{ output_image_d0 sc_out sc_lv 8 signal 1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "input_image_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":19, "type": "signal", "bundle":{"name": "input_image", "role": "address0" }} , 
 	{ "name": "input_image_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_image", "role": "ce0" }} , 
 	{ "name": "input_image_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "input_image", "role": "q0" }} , 
 	{ "name": "input_image_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":19, "type": "signal", "bundle":{"name": "input_image", "role": "address1" }} , 
 	{ "name": "input_image_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "input_image", "role": "ce1" }} , 
 	{ "name": "input_image_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "input_image", "role": "q1" }} , 
 	{ "name": "output_image_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":17, "type": "signal", "bundle":{"name": "output_image", "role": "address0" }} , 
 	{ "name": "output_image_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_image", "role": "ce0" }} , 
 	{ "name": "output_image_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "output_image", "role": "we0" }} , 
 	{ "name": "output_image_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":8, "type": "signal", "bundle":{"name": "output_image", "role": "d0" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6"],
		"CDFG" : "interpolate_image",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "2851841", "EstimateLatencyMax" : "2851841",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "input_image", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "output_image", "Type" : "Memory", "Direction" : "O"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_faddfsub_32ns_32ns_32_4_full_dsp_1_U1", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_fsub_32ns_32ns_32_4_full_dsp_1_U2", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_fmul_32ns_32ns_32_3_max_dsp_1_U3", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_fmul_32ns_32ns_32_3_max_dsp_1_U4", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_sitofp_32ns_32_4_1_U5", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.interpolate_image_sitofp_32s_32_4_1_U6", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	interpolate_image {
		input_image {Type I LastRead 30 FirstWrite -1}
		output_image {Type O LastRead -1 FirstWrite 49}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "2851841", "Max" : "2851841"}
	, {"Name" : "Interval", "Min" : "2851842", "Max" : "2851842"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	input_image { ap_memory {  { input_image_address0 mem_address 1 19 }  { input_image_ce0 mem_ce 1 1 }  { input_image_q0 mem_dout 0 8 }  { input_image_address1 MemPortADDR2 1 19 }  { input_image_ce1 MemPortCE2 1 1 }  { input_image_q1 MemPortDOUT2 0 8 } } }
	output_image { ap_memory {  { output_image_address0 mem_address 1 17 }  { output_image_ce0 mem_ce 1 1 }  { output_image_we0 mem_we 1 1 }  { output_image_d0 mem_din 1 8 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
