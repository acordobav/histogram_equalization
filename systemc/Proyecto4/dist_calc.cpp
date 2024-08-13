// Code your design here.
// Uncomment the next line for SystemC modules.
// #include <systemc>

//-----------------------------------------------------
#include "global_register_bank.hpp"
// #include "RegisterBank.cpp"
#include "memory_map.h"
#include "systemc.h"
#include <systemc-ams.h>

int hex_sensor[8] = {REG_DISTANCE0,
                     REG_DISTANCE1,
                     REG_DISTANCE2,
                     REG_DISTANCE3,
                     REG_DISTANCE4,
                     REG_DISTANCE5,
                     REG_DISTANCE6,
                     REG_DISTANCE7};

int hex_cam[8] = {REG_CAPTURE0,
                  REG_CAPTURE1,
                  REG_CAPTURE2,
                  REG_CAPTURE3,
                  REG_CAPTURE4,
                  REG_CAPTURE5,
                  REG_CAPTURE6,
                  REG_CAPTURE7};
int j = 0;
int i = 0;

SC_MODULE(dist_calc)
{
  sc_out<double> dist_cm;
  sc_out<double> calc_voltage;
  //  sca_tdf::sca_signal<bool> sens_active_cable("sens_active_cable");
  //  sc_out<bool > sens_active;
  sc_out<bool> sens_active;
  sc_out<const char *> sens_range;
  sc_out<bool> trigger;
  sc_in<double> echo_signal;
  sc_in<double> time_signal;
  sc_out<sc_int<32>> count_near;
  sc_out<sc_int<32>> count_half;
  sc_out<sc_int<32>> count_far;

  //-----------Internal variables-------------------
  sc_event d_capture, d_start, end_wait;
  sc_int<64> resultado, t2;

  sc_lv<32> control_register;
  sc_int<32> range1 = 4;
  sc_int<32> range2 = 100;
  sc_int<32> range3 = 200;
  sc_int<32> range4 = 300;
  sc_int<32> cuenta_near = 0;
  sc_int<32> cuenta_half = 0;
  sc_int<32> cuenta_far = 0;
  sc_int<32> zero_value = 0;
  sc_int<32> round = 0;
  double time_signal_value = 0;
  double distance_cm = 0;
  bool sensor_active = 0;

  int distance_out_module;
  bool sens_active_out_module;

  // Constructor
  SC_CTOR(dist_calc)
  {
    // SC_THREAD(pulse_trigger);
    SC_THREAD(pulse_echo);
    // SC_THREAD(end_wait_th);
  }

  //------------Code Starts Here-------------------------

  void delay_start()
  {
    d_start.notify(2, SC_US);
  }

  void e_wait()
  {
    end_wait.notify(0, SC_US);
  }

  void pulse_trigger()
  {
    wait(d_start);
    while (true)
    {
      trigger = 1;
      wait(10, SC_US);
      trigger = 0;
      wait(1000, SC_MS); // each 1s trigger signal will be raised
    }
  }

  void pulse_echo()
  {
    uint32_t wr_value = 0;
    wait(d_start);
    while (true)
    {
      if (echo_signal.read() != zero_value)
      {

        while (i < 8)
        {
          wr_value = 0x0;
          global_register_bank.write(hex_sensor[i], &wr_value, sizeof(wr_value));
          wait(3, SC_US);
          global_register_bank.write(hex_cam[i], &wr_value, sizeof(wr_value));
          wait(3, SC_US);
          // cout << "i :      " << i << endl;
          i = i + 1;
        }

        // cout << "echo_signal being read: " << echo_signal.read() << endl;
        wait(5, SC_US);
        time_signal_value = time_signal.read() * 1000000;
        wait(1, SC_US);
        distance_cm = time_signal_value * 0.034;
        wait(1, SC_US);
        round = static_cast<int>(std::round(distance_cm));
        // size_t index = get_register_index(0x10000);
        // registers.at(index) = value;
        int random = 0 + (rand() % (7 - 0 + 1));
        // cout << "nuevo INDEX:      " << random << endl;
        global_register_bank.write(hex_sensor[random], &round, sizeof(round));

        // wait(1, SC_US);
        wr_value = 0x1;
        global_register_bank.write(hex_cam[random], &wr_value, sizeof(wr_value));

        wait(1, SC_US);

        if (distance_cm != zero_value)
        {
          // dist_cm.write(distance_cm);
          distance_out_module = distance_cm;
          calc_voltage = distance_cm * 0.0016; // here!!!!
        }
        // printf("- - -PROBANDO voltaje - - - \n");
        // wait(1000, SC_US);
        sensor_range();
        // cout << "cam still  INDEX:        " << random << endl;
        // i=10;
      };
      wait(5000, SC_US);
    }
  }

  void sensor_range()
  {
    wait(30, SC_US);

    // printf("DIST CALC ");
    if ((distance_cm) >= range1 && (distance_cm) <= range2)
    {
      sens_range = "cercano";
      sensor_active = 1;
      // sens_active_out_module = sensor_active;
      // wait(1, SC_US);
      sens_active_out_module = sensor_active;
      wait(3, SC_MS);
      sensor_active = 0;
      // sens_active.write(sensor_active);
      // cout << "SENS ACTIVE 1 " << sensor_active << endl;
      // wait(3, SC_MS);
      // sensor_active = 0;
      cuenta_near = cuenta_near + 1;
      count_near = cuenta_near;
    }
    else if ((distance_cm) > range2 && (distance_cm) <= range3)
    {
      sens_range = "medio";
      sensor_active = 1;
      // wait(1, SC_US);
      sens_active_out_module = sensor_active;
      wait(3, SC_MS);
      sensor_active = 0;
      // sens_active_out_module = sensor_active;
      // sens_active.write(sensor_active);
      // cout << "SENS ACTIVE 2 " << sensor_active << endl;
      // wait(3, SC_MS);
      // sensor_active = 0;
      cuenta_half = cuenta_half + 1;
      count_half = cuenta_half;
    }
    else if ((distance_cm) > range3 && (distance_cm) <= range4)
    {
      sens_range = "lejano";

      sensor_active = 1;
      // sens_active_out_module = sensor_active;
      // wait(1, SC_US);
      sens_active_out_module = sensor_active;
      wait(3, SC_MS);
      sensor_active = 0;
      // sens_active.write(sensor_active);
      // cout << "SENS ACTIVE 3 " << sensor_active << endl;
      // wait(3, SC_MS);
      // sensor_active = 0;
      cuenta_far = cuenta_far + 1;
      count_far = cuenta_far;
    }
    else
    {
      sens_range = "fuera_de_rango";
      sensor_active = 0;
      sens_active_out_module = sensor_active;
      // sens_active.write(sensor_active);
    }

    // cout << "Inside" << endl;
    // cout << "Distance: " << dist_cm << endl;
    // cout << "Sensr Active: " << sens_active_out_module << endl;
  }

  int read_distance()
  {
    return distance_out_module;
  }

  bool read_sens_active()
  {
    return sens_active_out_module;
  }
};
