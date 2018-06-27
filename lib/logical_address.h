#ifndef LOGICAL_ADDRESS
#define LOGICAL_ADDRESS

#include <iostream>
#include <stdint.h>
#include <algorithm>


/**
 * Logical Addresses are an abstraction for component addresses and interfaces.
 * They are defined by the system as a virtual component location.
 * They allow components to communicate independent of their communication protocol.
 *
 * example: an i2c component can communicate with a USB component simply
 * by referring to it by its logical address
 */
struct LogicalAddress
{
  /**
   * LogicalAddress Constructor
   * @param subId the subnet that a component is assigned to
   * @param compId the unique identifier for a component within a subnet
   */
  LogicalAddress(uint16_t const subId = 0, uint16_t const compId = 0) : subnetId(subId), componentId(compId) {}

  /* subnetId  The subnet that a component is assigned to */
  uint16_t const subnetId;
  /* componentId The unique identifier for a component within a subnet */
  uint16_t const componentId;
};

/* 
 * Stream insertion operator overload that will format a LogicalAddress
 * into a nice string
 */
std::ostream& operator<<(std::ostream& o, const LogicalAddress& la);

/*
 * Equivalence is defined as two LogicalAddresses having the same subnet AND component ID
 */
bool operator==(const LogicalAddress& lhs, const LogicalAddress& rhs);

/*
 * This less-than comparison is actually a bit arbitrarily defined
 * and only exists so that LogicalAddresses can be ordered into
 * a nicely balanced binary tree
 */
bool operator<(const LogicalAddress& lhs, const LogicalAddress& rhs);

/*
 * This struct is used in the comparison operator 
 */
struct LogicalAddressCompare
{
  bool operator()(const LogicalAddress& lhs, const LogicalAddress& rhs) const
  {
    if (lhs.subnetId == rhs.subnetId)
    {
      return lhs.componentId < rhs.componentId;
    }
    else
    {
      return lhs.subnetId < rhs.subnetId;
    }
  }
};

#endif
