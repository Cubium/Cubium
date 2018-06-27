#ifndef COMPONENT_LIST
#define COMPONENT_LIST

#include "logical_address.h"
#include <component.hpp>
#include <vector>

/*
 * Quick and dirty little struct that just contains a component's
 * address and whether or not its healthy. 
 *
 * Note: healthiness is not yet implemented 
 */
struct ComponentInfo
{
  ComponentInfo(LogicalAddress la, bool h) : address(la), healthy(h) {}
  LogicalAddress address;
  bool healthy = true;
  // timeSinceLastHeartbeat
};

/* A basic list of components */
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
  ssize_t getSize() { return list.size(); }

protected:
  std::vector<ComponentInfo> list;
};

#endif
