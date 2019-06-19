#!/bin/sh
cd ~/cubium/hab/uvInternal/
./uvInternal.out &

cd ~/cubium/hab/subnetmanager
./subnet_driver.out &

cd ~/cubium/hab/uvExternal/
./uvExternal.out &

cd ~/cubium/hab/analog-temp1/
./analog-temp1.out &

cd ~/cubium/hab/analog-temp2/
./analog-temp2.out &

cd ~/cubium/hab/barometer/
./barometer.out &

cd ~/cubium/hab/battery/
./battery.out &

cd ~/cubium/hab/boom/
./boom.out &

cd ~/cubium/hab/camera/
./camera.out &

cd ~/cubium/hab/digital-temp/
./digital-temp.out &

cd ~/cubium/hab/filter/
./filter.out &

cd ~/cubium/hab/gyro/
./gyro.out &

cd ~/cubium/hab/light-external/
./light_ex.out &

cd ~/cubium/hab/light-internal/
./light_in.out &

cd ~/cubium/hab/magnetometer/
./magnetometer.out &

cd ~/cubium/hab/aggregator/
./aggregator.out &


