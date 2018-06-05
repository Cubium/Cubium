#!/bin/sh
cd ~/Cubium/hab/uvInternal/
./uvInternal.out &

cd ~/Cubium/hab/subnetmanager
./subnet_driver.out &

cd ~/Cubium/hab/uvExternal/
./uvExternal.out &

cd ~/Cubium/hab/analog-temp1/
./analog-temp1.out &

cd ~/Cubium/hab/analog-temp2/
./analog-temp2.out &

cd ~/Cubium/hab/barometer/
./barometer.out &

cd ~/Cubium/hab/battery/
./battery.out &

cd ~/Cubium/hab/boom/
./boom.out &

cd ~/Cubium/hab/camera/
./camera.out &

cd ~/Cubium/hab/digital-temp/
./digital-temp.out &

cd ~/Cubium/hab/filter/
./filter.out &

cd ~/Cubium/hab/gyro/
./gyro.out &

cd ~/Cubium/hab/light-external/
./light_ex.out &

cd ~/Cubium/hab/light-internal/
./light_in.out &

cd ~/Cubium/hab/magnetometer/
./magnetometer.out &

cd ~/Cubium/hab/aggregator/
./aggregator.out &


