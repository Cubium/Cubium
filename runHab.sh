#!/bin/bash

xterm -title "uv" -e "./hab/uv/uv" &
sleep 0.3
xterm -title "subnetmanager" -e "./hab/subnetmanager/subnet_driver" &
sleep 0.3
xterm -title "aggregator" -e "./hab/aggregator/aggregator" &
sleep 0.3
xterm -title "baro" -e "./hab/baro/baro" &
sleep 0.3
xterm -title "filter" -e "./hab/filter/filter" &
sleep 0.3
xterm -title "boom" -e "./hab/boom/boom" &
sleep 0.3
xterm -title "gps" -e "./hab/gps/gps" &
sleep 0.3
xterm -title "light" -e "./hab/light/light" &
sleep 0.3
xterm -title "radio" -e "./hab/radio/radio" &
sleep 0.3
xterm -title "rtc" -e "./hab/rtc/rtc" &
sleep 0.3
xterm -title "tempex" -e "./hab/tempex/tempex" &
sleep 0.3
xterm -title "tempin" -e "./hab/tempin/tempin" &
sleep 0.3
xterm -title "camera" -e "./hab/camera/camera" 
