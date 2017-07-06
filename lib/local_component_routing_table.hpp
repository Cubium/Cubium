#ifndef LOCAL_COMPONENT_ROUTING_TABLE_HPP
#define LOCAL_COMPONENT_ROUTING_TABLE_HPP

#include "logical_address.h"

#define LOCAL_SUBNET_MANAGER_PORT 8888

class LocalComponentRoutingTable : public RoutingTable
{
  int32_t getPhysicalAddress(LogicalAddress log) { return LOCAL_SUBNET_MANAGER_PORT; }
};

#endif
