
#ifndef PHYSICAL_COMMUNICATOR_HPP
#define PHYSICAL_COMMUNICATOR_HPP

#include <functional>

#include "logical_address.h"
#include "spa_message.h"

class PhysicalCommunicator
{
public:
  typedef void (*MessageCallback)(void *, uint32_t);

  PhysicalCommunicator(LogicalAddress la) : subnetAddress(la) {}

  virtual ~PhysicalCommunicator() {}
  virtual bool sendMsg(SpaMessage* message) { return false; }
  virtual void listen(std::function<void(void *, uint32_t)>) {}

  virtual LogicalAddress getSubnetAddress() { return subnetAddress; }

  virtual void insertToRoutingTable(LogicalAddress, uint32_t){};

  LogicalAddress subnetAddress;
};
#endif
