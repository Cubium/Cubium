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

  uint16_t const subnetId;
  uint16_t const componentId;
};

std::ostream& operator<<(std::ostream& o, const LogicalAddress& la);

bool operator==(const LogicalAddress& lhs, const LogicalAddress& rhs);

bool operator<(const LogicalAddress& lhs, const LogicalAddress& rhs);

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
