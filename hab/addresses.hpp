#ifndef ADDRESSES_HPP
#define ADDRESSES_HPP
#include <logical_address.h>

const LogicalAddress la_LSM(1, 0);
const LogicalAddress la_AGGREGATOR(1, 1);
const LogicalAddress la_RADIO(1, 2);
const LogicalAddress la_CAMERA(1, 3);
const LogicalAddress la_BOOM(1, 4);
const LogicalAddress la_FILTER(1, 5);
const LogicalAddress la_BAROMETER(1, 6);
const LogicalAddress la_BATTERY(1, 7);
const LogicalAddress la_DIGITAL_TEMP(1, 8);
const LogicalAddress la_ANALOG_TEMP1(1, 9);
const LogicalAddress la_ANALOG_TEMP2(1, 10);
const LogicalAddress la_UV_INTERNAL(1, 11);
const LogicalAddress la_UV_EXTERNAL(1, 12);
const LogicalAddress la_GYROSCOPE(1, 13);
const LogicalAddress la_MAGNETOMETER(1, 14);
const LogicalAddress la_LIGHT_INTERNAL(1, 15);
const LogicalAddress la_LIGHT_EXTERNAL(1, 16);

const int NUM_COMPONENTS = 16;
const int NUM_SUBSCRIPTIONS = 20;

#endif
