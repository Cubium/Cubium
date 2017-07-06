#include "logical_address.h"

bool operator==(const LogicalAddress &lhs, const LogicalAddress &rhs)
{
  return lhs.subnetId == rhs.subnetId && lhs.componentId == rhs.componentId;
}
