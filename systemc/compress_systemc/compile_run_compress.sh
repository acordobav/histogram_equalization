#!/bin/sh

# Archivo de log
LOG_FILE="simulation.log"

echo 'Compiling *.c *.cpp files' | tee -a "$LOG_FILE"
rm -rf compression.o | tee -a "$LOG_FILE"
export SYSTEMC_HOME=/usr/local/systemc-2.3.2/
export LD_LIBRARY_PATH="$SYSTEMC_HOME/lib-linux64"

g++ -I"$SYSTEMC_HOME/include" -L"$SYSTEMC_HOME/lib-linux64" compression_tb.cpp compression.cpp -lsystemc -ljpeg -o compression.o 2>&1 | tee -a "$LOG_FILE"

echo 'Simulation Started' | tee -a "$LOG_FILE"
./compression.o 2>&1 | tee -a "$LOG_FILE"
echo 'Simulation Ended' | tee -a "$LOG_FILE"

du -h *.jpg | tee -a "$LOG_FILE"