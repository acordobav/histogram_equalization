#!/bin/sh
echo 'Compiling *.c *cpp files'
rm -rf equalization.o
export SYSTEMC_HOME=/usr/local/systemc-2.3.4/
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 equalization_tb.cpp equalization.cpp  -lsystemc -lm -o equalization.o
echo 'Simulation Started'
./equalization.o
echo 'Simulation Ended'