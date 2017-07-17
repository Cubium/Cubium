// Utilizes routing table to map logical addresses to IP addresses, including other subnets

#ifndef LOCAL_COMMUNICATOR_HPP
#define LOCAL_COMMUNICATOR_HPP
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include "physical_communicator.hpp"
#include "socket/clientSocket.hpp"
#include "socket/serverSocket.hpp"
#include "routing_table.hpp"
#include "spa_message.h"

#define SERVER "127.0.0.1"

class LocalCommunicator : public PhysicalCommunicator
{
public:
  
  LocalCommunicator(
      cubiumServerSocket_t * sock, 
	  std::shared_ptr<RoutingTable> routingTable,
      LogicalAddress la) : serverSock(sock), routingTable(routingTable), PhysicalCommunicator(la) { ; }

  LocalCommunicator(
      cubiumClientSocket_t * sock, 
	  std::shared_ptr<RoutingTable> routingTable,
      LogicalAddress la) : clientSock(sock), routingTable(routingTable), PhysicalCommunicator(la) { ; }
  
  LocalCommunicator(
      cubiumServerSocket_t * sock,
      LogicalAddress la) : serverSock(sock), routingTable(nullptr), PhysicalCommunicator(la) { ; }
  
  virtual void handleFailure();
  virtual bool serverSend(SpaMessage* message);
  virtual bool clientSend(SpaMessage* message);

  void clientConnect(SpaMessage *, size_t, std::function<void(cubiumClientSocket_t *)>);

  virtual void listen(std::function<void(cubiumServerSocket_t *)>);
  virtual void insertToRoutingTable(LogicalAddress log, uint32_t);

protected:
  cubiumServerSocket_t * serverSock;
  cubiumClientSocket_t * clientSock;
  std::shared_ptr<RoutingTable> routingTable;
};

#endif
