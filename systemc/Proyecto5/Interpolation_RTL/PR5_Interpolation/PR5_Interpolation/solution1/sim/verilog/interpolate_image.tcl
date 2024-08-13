
log_wave [get_objects -filter {type == in_port || type == out_port || type == inout_port || type == port} /apatb_interpolate_image_top/AESL_inst_interpolate_image/*]
set designtopgroup [add_wave_group "Design Top Signals"]
set coutputgroup [add_wave_group "C Outputs" -into $designtopgroup]
set output_image_group [add_wave_group output_image(memory) -into $coutputgroup]
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/output_image_d0 -into $output_image_group -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/output_image_we0 -into $output_image_group -color #ffff00 -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/output_image_ce0 -into $output_image_group -color #ffff00 -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/output_image_address0 -into $output_image_group -radix hex
set cinputgroup [add_wave_group "C Inputs" -into $designtopgroup]
set input_image_group [add_wave_group input_image(memory) -into $cinputgroup]
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_q1 -into $input_image_group -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_ce1 -into $input_image_group -color #ffff00 -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_address1 -into $input_image_group -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_q0 -into $input_image_group -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_ce0 -into $input_image_group -color #ffff00 -radix hex
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/input_image_address0 -into $input_image_group -radix hex
set blocksiggroup [add_wave_group "Block-level IO Handshake" -into $designtopgroup]
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_start -into $blocksiggroup
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_done -into $blocksiggroup
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_idle -into $blocksiggroup
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_ready -into $blocksiggroup
set resetgroup [add_wave_group "Reset" -into $designtopgroup]
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_rst -into $resetgroup
set clockgroup [add_wave_group "Clock" -into $designtopgroup]
add_wave /apatb_interpolate_image_top/AESL_inst_interpolate_image/ap_clk -into $clockgroup
save_wave_config interpolate_image.wcfg
run all
quit

