
#ifndef PHYSICAL_COMMUNICATOR_HPP
#define PHYSICAL_COMMUNICATOR_HPP

#include <functional>

#include "logical_address.h"
#include "spa_message.h"
#include "socket/clientSocket.hpp"
#include "socket/serverSocket.hpp"
#include <iostream>

class PhysicalCommunicator
{
public:
  typedef void (*MessageCallback)(void *, uint32_t);

  PhysicalCommunicator(LogicalAddress la) : subnetAddress(la) {}

  virtual ~PhysicalCommunicator() {}
  virtual bool sendMsg(SpaMessage* message) { return false; }
  virtual void listen(std::function<void(cubiumServerSocket_t*)>) { std::cout << "I should not be called!\n"; }

  virtual LogicalAddress getSubnetAddress() { return subnetAddress; }

  virtual void insertToRoutingTable(LogicalAddress, uint32_t){};

  virtual void clientConnect(SpaMessage *, std::function<void(cubiumClientSocket_t*)>) {}

  LogicalAddress subnetAddress;
};
#endif
