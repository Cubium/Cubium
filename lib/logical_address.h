#ifndef LOGICAL_ADDRESS
#define LOGICAL_ADDRESS

#include <iostream>
#include <stdint.h>

struct LogicalAddress
{
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
