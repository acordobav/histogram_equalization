#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/jpv/Msc/DisAltNiv/vivadoFiles/Example/OtherExampleForDistance/Distance/Calc_distance/solution1/.autopilot/db/a.g.bc ${1+"$@"}
