#ifndef ROUTING_TABLE_HPP
#define ROUTING_TABLE_HPP

#include "logical_address.h"
#include <iostream>
#include <map>

/* The glorious, all-holy routing table.
 * Contains information on which port each logical-address maps to
 * and is used widely across the network for messaging
 */
template <typename T>
class RoutingTable
{
public:
  /* Dirty dirty default constructor */
  RoutingTable() {}

  /* 
   * Construct the routing table by giving it its first entry.
   * @param log      The logical address of the first entry
   * @param physAddr The physical address of the first entry
   */
  RoutingTable(LogicalAddress log, T physAddr)
  {
    if (!insert(log, physAddr))
      std::cerr << "Routing Table Construction Failure." << std::endl;
  }

  /*
   * Insert an entry into the routing table.
   * @param log      The logical address of the entry
   * @param physAddr The physical address of the entry
   */
  bool insert(LogicalAddress log, T physAddr)
  {
    routingTable.insert(std::pair<LogicalAddress, T>(log, physAddr));
    return true;
  }

  /*
   * Check to see if a given LogicalAddress is in the table.
   * @param log The LogicalAddress in question
   */
  bool exists(LogicalAddress log)
  {
    typename std::map<LogicalAddress, T, LogicalAddressCompare>::iterator i = routingTable.begin();
    for (i = routingTable.begin(); i != routingTable.end(); ++i)
    {
      if (i->first == log)
      {
        return true;
      }
    }
    return false;
  }

  /*
   * Check is the table is empty
   */
  bool isEmpty() { return routingTable.empty(); }

  /*
   * Get the size of the table
   */
  int size() { return routingTable.size(); }

  /*
   * Get the physical address of a given logical address
   * @param log The LogicalAddress in question
   */
  T getPhysicalAddress(LogicalAddress log)
  {
    if (exists(log))
    {
      return routingTable[log];
    }
    else
    {
      std::cout << "Failed to find address: " << log << std::endl;
      perror("Entry in routing table does not exist!");
      exit(1);
    }
  }

  /*
   * Get a copy of the entire Routing Table
   */
  std::map<LogicalAddress, T, LogicalAddressCompare> getTable()
  {
    return routingTable;
  }

protected:
  /* routingTable  Under the hood, the routingTable is really just a std::map */
  std::map<LogicalAddress, T, LogicalAddressCompare> routingTable;
};

#endif
