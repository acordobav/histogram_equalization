create_project prj -part xc7vx485t-ffg1157-1 -force
set_property target_language verilog [current_project]
set vivado_ver [version -short]
set COE_DIR "../../syn/verilog"
source "/home/jpv/Msc/DisAltNiv/Proyectos/Proyecto5/vivado_interpolation/PR5_Interpolation/PR5_Interpolation/solution1/syn/verilog/interpolate_image_ap_fmul_1_max_dsp_32_ip.tcl"
if {[regexp -nocase {2015\.3.*} $vivado_ver match] || [regexp -nocase {2016\.1.*} $vivado_ver match]} {
    extract_files -base_dir "./prjsrcs/sources_1/ip" [get_files -all -of [get_ips interpolate_image_ap_fmul_1_max_dsp_32]]
}
source "/home/jpv/Msc/DisAltNiv/Proyectos/Proyecto5/vivado_interpolation/PR5_Interpolation/PR5_Interpolation/solution1/syn/verilog/interpolate_image_ap_sitofp_2_no_dsp_32_ip.tcl"
if {[regexp -nocase {2015\.3.*} $vivado_ver match] || [regexp -nocase {2016\.1.*} $vivado_ver match]} {
    extract_files -base_dir "./prjsrcs/sources_1/ip" [get_files -all -of [get_ips interpolate_image_ap_sitofp_2_no_dsp_32]]
}
source "/home/jpv/Msc/DisAltNiv/Proyectos/Proyecto5/vivado_interpolation/PR5_Interpolation/PR5_Interpolation/solution1/syn/verilog/interpolate_image_ap_faddfsub_2_full_dsp_32_ip.tcl"
if {[regexp -nocase {2015\.3.*} $vivado_ver match] || [regexp -nocase {2016\.1.*} $vivado_ver match]} {
    extract_files -base_dir "./prjsrcs/sources_1/ip" [get_files -all -of [get_ips interpolate_image_ap_faddfsub_2_full_dsp_32]]
}
source "/home/jpv/Msc/DisAltNiv/Proyectos/Proyecto5/vivado_interpolation/PR5_Interpolation/PR5_Interpolation/solution1/syn/verilog/interpolate_image_ap_fsub_2_full_dsp_32_ip.tcl"
if {[regexp -nocase {2015\.3.*} $vivado_ver match] || [regexp -nocase {2016\.1.*} $vivado_ver match]} {
    extract_files -base_dir "./prjsrcs/sources_1/ip" [get_files -all -of [get_ips interpolate_image_ap_fsub_2_full_dsp_32]]
}
