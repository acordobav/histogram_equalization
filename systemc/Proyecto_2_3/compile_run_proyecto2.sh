#!/bin/sh
echo 'Compiling *.c *cpp files'
rm -rf top.o
export SYSTEMC_HOME=/usr/local/systemc-2.3.4/
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib-linux64
export LD_LIBRARY_PATH=/usr/local/systemc-ams-2.3.4/lib-linux64/:$SYSTEMC_HOME/lib-linux64/
g++ -I$SYSTEMC_HOME/include -L$SYSTEMC_HOME/lib-linux64 -I/usr/local/systemc-ams-2.3.4/include -L/usr/local/systemc-ams-2.3.4/lib-linux64 topUS.cpp  -lsystemc -lsystemc-ams -lm -o top.o
echo 'Simulation Started'
./top.o
echo 'Simulation Ended'
