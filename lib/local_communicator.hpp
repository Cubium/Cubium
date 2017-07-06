// Utilizes routing table to map logical addresses to IP addresses, including other subnets

#ifndef LOCAL_COMMUNICATOR_HPP
#define LOCAL_COMMUNICATOR_HPP
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include "physical_communicator.hpp"
#include "platform_abstraction/socket/server_socket.hpp"
#include "routing_table.hpp"
#include "spa_message.hpp"

#define SERVER "127.0.0.1"

class LocalCommunicator : public PhysicalCommunicator
{
public:
  
  LocalCommunicator(
      std::shared_ptr<ServerSocket> sock, 
	  std::shared_ptr<RoutingTable> routingTable,
      LogicalAddress la) : sock(sock), routingTable(routingTable), PhysicalCommunicator(la) { ; }
  
  LocalCommunicator(
      std::shared_ptr<ServerSocket> sock,
      LogicalAddress la) : sock(sock), routingTable(nullptr), PhysicalCommunicator(la) { ; }
  
  virtual void handleFailure();
  virtual bool sendMsg(std::shared_ptr<SpaMessage> message);

  virtual void listen(std::function<void(uint8_t *, uint32_t)>);
  virtual void insertToRoutingTable(LogicalAddress log, uint32_t);

protected:
  std::shared_ptr<ServerSocket> sock;
  std::shared_ptr<RoutingTable> routingTable;
};

#endif
