#!/bin/bash

make -j

xterm -title "Subnet Manager" -e "./simple_system/subnetmanager/subnet_driver" &
xterm -title "Component A" -e "./simple_system/component-A" &
xterm -title "Component B" -e "./simple_system/component-B" &

exit
