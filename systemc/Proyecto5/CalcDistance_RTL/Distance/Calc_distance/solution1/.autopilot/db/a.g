#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/jpv/Msc/DisAltNiv/Repo/histogram_equalization/systemc/Proyecto5/CalcDistance_RTL/Distance/Calc_distance/solution1/.autopilot/db/a.g.bc ${1+"$@"}
