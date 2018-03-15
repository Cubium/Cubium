#ifndef COMPONENT_LIST
#define COMPONENT_LIST

#include "logical_address.h"
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
  ComponentList() {}
  ComponentList(LogicalAddress l) { add(l); }
  void add(LogicalAddress la)
  {
    /* Allocates just enough memory for what it is to contain */
    list.reserve(list.size() + 1);
    list.emplace_back(la, true);
  }
  LogicalAddress getAddress(int i) { return list[i].address; }

protected:
  std::vector<ComponentInfo> list;
};

#endif
