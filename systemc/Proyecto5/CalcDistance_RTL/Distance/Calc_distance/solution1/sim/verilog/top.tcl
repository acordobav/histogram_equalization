
log_wave -r /
set designtopgroup [add_wave_group "Design Top Signals"]
set coutputgroup [add_wave_group "C Outputs" -into $designtopgroup]
set sens_range_group [add_wave_group sens_range(wire) -into $coutputgroup]
add_wave /apatb_top_top/AESL_inst_top/sens_range_ap_vld -into $sens_range_group -color #ffff00 -radix hex
add_wave /apatb_top_top/AESL_inst_top/sens_range -into $sens_range_group -radix hex
set calc_voltage_group [add_wave_group calc_voltage(wire) -into $coutputgroup]
add_wave /apatb_top_top/AESL_inst_top/calc_voltage_ap_vld -into $calc_voltage_group -color #ffff00 -radix hex
add_wave /apatb_top_top/AESL_inst_top/calc_voltage -into $calc_voltage_group -radix hex
set dist_cm_group [add_wave_group dist_cm(wire) -into $coutputgroup]
add_wave /apatb_top_top/AESL_inst_top/dist_cm_ap_vld -into $dist_cm_group -color #ffff00 -radix hex
add_wave /apatb_top_top/AESL_inst_top/dist_cm -into $dist_cm_group -radix hex
set cinputgroup [add_wave_group "C Inputs" -into $designtopgroup]
set simulated_time_group [add_wave_group simulated_time(wire) -into $cinputgroup]
add_wave /apatb_top_top/AESL_inst_top/simulated_time -into $simulated_time_group -radix hex
set echo_group [add_wave_group echo(wire) -into $cinputgroup]
add_wave /apatb_top_top/AESL_inst_top/echo -into $echo_group -radix hex
set blocksiggroup [add_wave_group "Block-level IO Handshake" -into $designtopgroup]
add_wave /apatb_top_top/AESL_inst_top/ap_start -into $blocksiggroup
add_wave /apatb_top_top/AESL_inst_top/ap_done -into $blocksiggroup
add_wave /apatb_top_top/AESL_inst_top/ap_idle -into $blocksiggroup
add_wave /apatb_top_top/AESL_inst_top/ap_ready -into $blocksiggroup
set resetgroup [add_wave_group "Reset" -into $designtopgroup]
add_wave /apatb_top_top/AESL_inst_top/ap_rst -into $resetgroup
set clockgroup [add_wave_group "Clock" -into $designtopgroup]
add_wave /apatb_top_top/AESL_inst_top/ap_clk -into $clockgroup
set testbenchgroup [add_wave_group "Test Bench Signals"]
set tbinternalsiggroup [add_wave_group "Internal Signals" -into $testbenchgroup]
set tb_simstatus_group [add_wave_group "Simulation Status" -into $tbinternalsiggroup]
set tb_portdepth_group [add_wave_group "Port Depth" -into $tbinternalsiggroup]
add_wave /apatb_top_top/AUTOTB_TRANSACTION_NUM -into $tb_simstatus_group -radix hex
add_wave /apatb_top_top/ready_cnt -into $tb_simstatus_group -radix hex
add_wave /apatb_top_top/done_cnt -into $tb_simstatus_group -radix hex
add_wave /apatb_top_top/LENGTH_echo -into $tb_portdepth_group -radix hex
add_wave /apatb_top_top/LENGTH_simulated_time -into $tb_portdepth_group -radix hex
add_wave /apatb_top_top/LENGTH_dist_cm -into $tb_portdepth_group -radix hex
add_wave /apatb_top_top/LENGTH_calc_voltage -into $tb_portdepth_group -radix hex
add_wave /apatb_top_top/LENGTH_sens_range -into $tb_portdepth_group -radix hex
set tbcoutputgroup [add_wave_group "C Outputs" -into $testbenchgroup]
set tb_sens_range_group [add_wave_group sens_range(wire) -into $tbcoutputgroup]
add_wave /apatb_top_top/sens_range_ap_vld -into $tb_sens_range_group -color #ffff00 -radix hex
add_wave /apatb_top_top/sens_range -into $tb_sens_range_group -radix hex
set tb_calc_voltage_group [add_wave_group calc_voltage(wire) -into $tbcoutputgroup]
add_wave /apatb_top_top/calc_voltage_ap_vld -into $tb_calc_voltage_group -color #ffff00 -radix hex
add_wave /apatb_top_top/calc_voltage -into $tb_calc_voltage_group -radix hex
set tb_dist_cm_group [add_wave_group dist_cm(wire) -into $tbcoutputgroup]
add_wave /apatb_top_top/dist_cm_ap_vld -into $tb_dist_cm_group -color #ffff00 -radix hex
add_wave /apatb_top_top/dist_cm -into $tb_dist_cm_group -radix hex
set tbcinputgroup [add_wave_group "C Inputs" -into $testbenchgroup]
set tb_simulated_time_group [add_wave_group simulated_time(wire) -into $tbcinputgroup]
add_wave /apatb_top_top/simulated_time -into $tb_simulated_time_group -radix hex
set tb_echo_group [add_wave_group echo(wire) -into $tbcinputgroup]
add_wave /apatb_top_top/echo -into $tb_echo_group -radix hex
save_wave_config top.wcfg
run all
quit

