#ifndef LOCAL_COMMUNICATOR_HPP
#define LOCAL_COMMUNICATOR_HPP
#include "routing_table.hpp"
#include "socket/clientSocket.hpp"
#include "socket/serverSocket.hpp"
#include "spa_message.h"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#define SERVER "127.0.0.1"

class LocalCommunicator
{
public:
  LocalCommunicator(
      cubiumServerSocket_t* sock,
      std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable,
      LogicalAddress la) : serverSock(sock), clientSock(nullptr), routingTable(routingTable) { ; }

  LocalCommunicator(
      cubiumClientSocket_t* sock,
      std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable,
      LogicalAddress la) : clientSock(sock), serverSock(nullptr), routingTable(routingTable) { ; }

  LocalCommunicator(
      cubiumServerSocket_t* sock,
      LogicalAddress la) : serverSock(sock), clientSock(nullptr), routingTable(nullptr) { ; }

  virtual bool sendMsg(SpaMessage* msg, ssize_t len);

  virtual void handleFailure(std::string);
  virtual bool serverSend(SpaMessage* message, ssize_t);
  virtual bool clientSend(SpaMessage* message, ssize_t);

  void clientConnect(SpaMessage*, size_t, std::function<void(cubiumClientSocket_t*)>);
  void initSubDialogue(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback);

  void clientListen(std::function<void(cubiumClientSocket_t*)>, uint8_t);

  virtual void listen(std::function<int(cubiumServerSocket_t*)>);
  virtual void listen(std::function<void(cubiumClientSocket_t*)>);

  void setServerSock(cubiumServerSocket_t* s) { serverSock = s; }

  void printTable()
  {
    std::cout << '\n';
    std::cout << "  LA  |  port" << '\n'
              << "---------------" << '\n';

    auto table = routingTable->getTable();

    for (auto it = table.cbegin(); it != table.cend(); ++it)
    {
      std::cout << "(" << it->first.subnetId << "," << it->first.componentId << ") |  " << it->second.from.sin_port << '\n';
    }

    std::cout << '\n';
  }

protected:
  cubiumServerSocket_t* serverSock;
  cubiumClientSocket_t* clientSock;
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
