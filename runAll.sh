#!/bin/bash

xterm -title "Subnet Manager" -e "./demo/subnetmanager/subnet_driver" &
xterm -title "Light Sensor" -e "./demo/light/light" &
xterm -title "Median Filter" -e "./demo/medianFilter/medianFilter" &
xterm -title "Solar Array" -e "./demo/solarArray/solarArray" &
xterm -title "Temperature Sensor" -e "./demo/temperature/temperature" 
