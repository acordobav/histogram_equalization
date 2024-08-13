#!/bin/sh

# Archivo de log
LOG_FILE="simulation.log"

echo 'Compiling *.c *cpp files' | tee -a "$LOG_FILE"
rm -rf top.o | tee -a "$LOG_FILE"
export SYSTEMC_HOME=/usr/local/systemc-2.3.4/
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64
export LD_LIBRARY_PATH=/usr/local/systemc-ams-2.3.4/lib-linux64/:$SYSTEMC_HOME/lib-linux64/
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 -I/usr/local/systemc-ams-2.3.4/include -L/usr/local/systemc-ams-2.3.4/lib-linux64 tlmProyecto2.cpp  -lsystemc -lsystemc-ams -g -lm -o top.o 2>&1 | tee -a "$LOG_FILE"

echo 'Simulation Started' | tee -a "$LOG_FILE"
./top.o | tee -a "$LOG_FILE"
echo 'Simulation Ended' | tee -a "$LOG_FILE"
