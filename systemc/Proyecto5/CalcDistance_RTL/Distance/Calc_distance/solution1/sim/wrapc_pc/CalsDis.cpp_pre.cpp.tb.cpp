// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
# 1 "/home/jpv/Msc/DisAltNiv/vivadoFiles/Example/OtherExampleForDistance/files/CalsDis.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/jpv/Msc/DisAltNiv/vivadoFiles/Example/OtherExampleForDistance/files/CalsDis.cpp"

# 1 "/home/jpv/Msc/DisAltNiv/vivadoFiles/Example/OtherExampleForDistance/files/CalsDis.h" 1




void top(int echo, double simulated_time_microseconds, int *dist_cm, double *calc_voltage, int *sens_range);
# 3 "/home/jpv/Msc/DisAltNiv/vivadoFiles/Example/OtherExampleForDistance/files/CalsDis.cpp" 2
void top(int echo, double simulated_time, int *dist_cm, double *calc_voltage, int *sens_range) {
 *dist_cm = 0;
 *calc_voltage = 0.0;
 *sens_range = 0;

 if (echo == 1) {
  *dist_cm = (int)(simulated_time * 0.034);
  *calc_voltage = *dist_cm * 0.0016;

  if (*dist_cm >= 4 && *dist_cm <= 100) {
   *sens_range = 1;
  } else if (*dist_cm > 100 && *dist_cm <= 200) {
   *sens_range = 2;
  } else if (*dist_cm > 200 && *dist_cm <= 300) {
   *sens_range = 3;
  } else {
   *sens_range = 0;
  }
 }
}
