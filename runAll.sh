#!/bin/bash

xterm -title "Subnet Manager" -e "./demo/subnetmanager/subnet_driver" &
xterm -title "Subnet Manager" -e "./demo/light/light" &
xterm -title "Subnet Manager" -e "./demo/medianFilter/medianFilter" &
xterm -title "Subnet Manager" -e "./demo/solarArray/solarArray" &
xterm -title "Subnet Manager" -e "./demo/temperature/temperature" 
