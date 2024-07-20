#!/bin/sh
echo 'Compiling *.c *cpp files'
rm -rf top.o
export SYSTEMC_HOME=/usr/local/systemc-2.3.4/
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 top.cpp  -lsystemc -lm -g -o top.o
echo 'Simulation Started'
# gdb ./top.o
./top.o
echo 'Simulation Ended'
