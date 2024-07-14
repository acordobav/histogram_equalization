#!/bin/sh
echo 'Compiling *.c *cpp files'
rm -rf camara_sensor-tdf.o
export SYSTEMC_HOME=/usr/local/systemc-2.3.4/
export LD_LIBRARY_PATH=/usr/local/systemc-ams-2.3.4/lib-linux64/:$SYSTEMC_HOME/lib-linux64/
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 -I/usr/local/systemc-ams-2.3.4/include -L/usr/local/systemc-ams-2.3.4/lib-linux64 camara_sensor_tb.cpp -lsystemc -lsystemc-ams -lm -o camara_sensor-tdf.o
echo 'Simulation Started'
./camara_sensor-tdf.o
echo 'Simulation Ended'

du -h *.jpg | tee -a "$LOG_FILE"
