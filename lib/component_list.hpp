#ifndef COMPONENT_LIST
#define COMPONENT_LIST

#include "logical_address.hpp"
#include <component.hpp>
#include <vector>

struct ComponentInfo
{
  ComponentInfo(LogicalAddress la, bool h) : address(la), healthy(h) {}
  LogicalAddress address;
  bool healthy = true;
  // timeSinceLastHeartbeat
};

class ComponentList
{
public:
  void add(LogicalAddress la) { list.push_back(ComponentInfo(la, true)); }
  LogicalAddress getAddress(int i) { return list[i].address; }
protected:
  std::vector<ComponentInfo> list;
};

#endif
