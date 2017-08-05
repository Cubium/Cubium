#!/bin/bash

make -j

xterm -title "Subnet Manager" -e "./demo/subnetmanager/subnet_driver" &
xterm -title "Light Sensor" -e "./demo/light/light" &
xterm -title "Temperature Sensor" -e "./demo/temperature/temperature" &
xterm -title "Median Filter" -e "./demo/medianFilter/medianFilter" &
xterm -title "Solar Array" -e "./demo/solarArray/solarArray" 

exit
