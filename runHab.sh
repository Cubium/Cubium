#!/bin/bash

xterm -title "aggregator" -e "./hab/aggregator/aggregator" &
sleep 1
#sleep 1
#xterm -title "baro" -e "./hab/baro/baro" &
#sleep 1
#xterm -title "filter" -e "./hab/filter/filter" &
#sleep 1
#xterm -title "boom" -e "./hab/boom/boom" &
#sleep 1
#xterm -title "camera" -e "./hab/camera/camera" &
#sleep 1
xterm -title "gps" -e "./hab/gps/gps" &
sleep 1
#xterm -title "gyro" -e "./hab/gyro/gyro" &
#sleep 1
#xterm -title "light" -e "./hab/light/light" &
#sleep 1
#xterm -title "radio" -e "./hab/radio/radio" &
#sleep 1
#xterm -title "rtc" -e "./hab/rtc/rtc" &
#sleep 1
xterm -title "tempex" -e "./hab/tempex/tempex" &
sleep 1
xterm -title "tempin" -e "./hab/tempin/tempin" 
sleep 1
#xterm -title "subnetmanager" -e "./hab/subnetmanager/subnet_driver"  &
#xterm -title "uv" -e "./hab/uv/uv" 

exit
