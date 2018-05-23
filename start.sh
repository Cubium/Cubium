#!/bin/sh
cd hab/uvInternal/
./uvInternal.out &

~/Cubium/hab/subnetmanager/subnet_driver.out &

cd ~/Cubium/hab/uvExternal/
./uvExternal.out &

cd ~/Cubium/hab/light/
./light.out &

cd ~/Cubium/hab/rtc/
./rtc.out &

cd ~/Cubium/hab/temp/
./temp.out &

cd ~/Cubium/hab/filter/
./filter.out &

cd ~/Cubium/hab/boom/
./boom.out &

cd ~/Cubium/hab/camera/
./camera.out &

cd ~/Cubium/hab/aggregator/
./aggregator.out &
