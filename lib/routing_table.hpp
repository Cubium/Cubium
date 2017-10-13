#ifndef ROUTING_TABLE_HPP
#define ROUTING_TABLE_HPP

#include "logical_address.h"
#include <iostream>
#include <map>

template <typename T>
class RoutingTable
{
public:
  RoutingTable() {}

  RoutingTable(LogicalAddress log, T physAddr)
  {
    if (!insert(log, physAddr))
      std::cerr << "Routing Table Construction Failure." << std::endl;
  }

  bool insert(LogicalAddress log, T physAddr)
  {
    routingTable.insert(std::pair<LogicalAddress, T>(log, physAddr));
    //routingTable[log] = port;
    return true;
  }

  bool exists(LogicalAddress log)
  {
    typename std::map<LogicalAddress, T, LogicalAddressCompare>::iterator i = routingTable.begin();
    for (i = routingTable.begin(); i != routingTable.end(); ++i)
    {
      if (i->first == log)
      {
        //        std::cout << "(RoutingTable::exists)Found Address with port" << i->second << std::endl;
        return true;
      }
    }
    return false;
  }

  bool isEmpty() { return routingTable.empty(); }

  int size() { return routingTable.size(); }

  /*
    if (routingTable.find(log) == routingTable.end())
    {
      return false;
    }
    return true;
  }
*/

  virtual T getPhysicalAddress(LogicalAddress log)
  {
 //   std::cout << "Searching for address: " << log << "...\n";
    if (exists(log))
    {
      return routingTable[log];
    }
    else
    {
      perror("Entry in routing table does not exist!");
      exit(1);
    }
  }

  std::map<LogicalAddress, T, LogicalAddressCompare> getTable()
  {
    return routingTable;
  }

protected:
  std::map<LogicalAddress, T, LogicalAddressCompare> routingTable;
};

#endif
