#ifndef ADDRESSES_HPP
#define ADDRESSES_HPP
#include <logical_address.h>

const LogicalAddress la_LSM(1, 0);
const LogicalAddress la_AGGREGATOR(1, 1);
const LogicalAddress la_RADIO(1, 2);
const LogicalAddress la_TEMP(1, 3);
const LogicalAddress la_RTC(1, 4);
const LogicalAddress la_UV_INTERNAL(1, 5);
const LogicalAddress la_UV_EXTERNAL(1, 6);
const LogicalAddress la_LIGHT(1, 7);
const LogicalAddress la_GYRO(1, 8);
const LogicalAddress la_FILTER(1, 9);
const LogicalAddress la_BOOM(1, 10);
const LogicalAddress la_CAMERA(1, 11);
const LogicalAddress la_BATTERY(1, 12);

const int NUM_COMPONENTS = 11;
const int NUM_SUBSCRIPTIONS = 14;

#endif
