#!/bin/bash

make -j

xterm -title "Subnet Manager" -e "./demo/subnetmanager/subnet_driver" &
xterm -title "Light Sensor" -e "./demo/light/light" &
xterm -title "Temperature Sensor" -e "./demo/temperature/temperature" &
xterm -title "Median Filter" -e "./demo/medianFilter/medianFilter | perl ./demo/driveGnuPlotStreams.pl 4 2 25 25 -273 100 0 50 500+300+0+0 500+300+0+0 Temperature Light FilteredTemp FilteredLight 1 0 0 1" &
xterm -title "Solar Array" -e "./demo/solarArray/solarArray" 

exit
