#include "logical_address.h"

bool operator==(const LogicalAddress& lhs, const LogicalAddress& rhs)
{
  return lhs.subnetId == rhs.subnetId && lhs.componentId == rhs.componentId;
}

bool operator<(const LogicalAddress& lhs, const LogicalAddress& rhs)
{
  return lhs.subnetId <= rhs.subnetId && lhs.componentId < rhs.componentId;
}

std::ostream& operator<<(std::ostream& o, LogicalAddress const& la)
{
  o << "(" << la.subnetId << ", " << la.componentId << ")";
  return o;
}
