#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/jpv/Msc/DisAltNiv/Proyectos/Proyecto5/vivado_interpolation/PR5_Interpolation/PR5_Interpolation/solution1/.autopilot/db/a.g.bc ${1+"$@"}
