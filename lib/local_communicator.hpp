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

/* Always localhost! */
#define SERVER "127.0.0.1"

/*
 * The communicator object that components use to send messages.
 * The name "LocalCommunicator" is a vestigial artifact from when
 * there were multiple types of communicators. They are now all
 * merged into one monolith.
 */
class LocalCommunicator
{
public:
  /*
   * Constructor for the subnet manager's communicator. 
   * Takes a serverSocket and makes its clientSocket a nullptr.
   *
   * @param sock         The server socket
   * @param routingTable The shared routing table
   * @param la           The LogicalAddress of the subnet manager
   */
  LocalCommunicator(
      cubiumServerSocket_t* sock,
      std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable,
      LogicalAddress la) : serverSock(sock), clientSock(nullptr), routingTable(routingTable) { ; }

  /*
   * Constructor for component communicators.
   * Takes a clientSocket and makes its serverSocket a nullptr.
   *
   * @param sock         The client socket
   * @param routingTable The shared routing table
   * @param la           The LogicalAddress of the component the communicator serves
   */
  LocalCommunicator(
      cubiumClientSocket_t* sock,
      std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable,
      LogicalAddress la) : clientSock(sock), serverSock(nullptr), routingTable(routingTable) { ; }

  /*
   * Deprecated.
   * TODO remove
   */
  LocalCommunicator(
      cubiumServerSocket_t* sock,
      LogicalAddress la) : serverSock(sock), clientSock(nullptr), routingTable(nullptr) { ; }


  /* 
   * Send a message through the socket.
   * Function is generic to which kind of socket the communicator serves.
   *
   * @param msg The message
   * @param len The length of the message (typically provided by sizeof(msg))
   */
  virtual bool sendMsg(SpaMessage* msg, ssize_t len);

  /*
   * Print an error message.
   *
   * @param message The error message to print
   */
  virtual void handleFailure(std::string message);

  /*
   * Send a message through the server socket if it exists.
   *
   * @param message The message
   * @param len     The length of the message (typically provided by sizeof(message))
   */
  virtual bool serverSend(SpaMessage* message, ssize_t len);

  /*
   * Send a message through the server socket if it exists.
   *
   * @param message The message
   * @param len     The length of the message (typically provided by sizeof(message))
   */
  virtual bool clientSend(SpaMessage* message, ssize_t len);

  /*
   * (component-serving communicators only)
   * Initiate a dialogue with the subnet manager to register the component.
   *
   * @param message  The LocalHello
   * @param len      The sizeof(LocalHello)
   * @param callback The callback function that is called upon receiving a message in the dialogue
   */
  void clientConnect(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback);

  /*
   * (component-serving communicators only)
   * Initiate a dialogue with another component to subscribe to its data.
   *
   * @param message  The SubscriptionRequest
   * @param len      The sizeof(SubscriptionRequest)
   * @param callback The callback function that is called upon receiving a message in the dialogue
   */
  void initSubDialogue(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback);

  /*
   * Listen for messages coming in through the client socket.
   *
   * @param func   The callback function called when a message comes in
   * @param exitOp Listening function terminates when it receives a message with this opcode
   */
  void clientListen(std::function<void(cubiumClientSocket_t*)> func, uint8_t exitOp);

  /*
   * Convenient overloaded function for listening.
   * @param messageHandler The callback function when a message is received.
   */
  virtual void listen(std::function<int(cubiumServerSocket_t*)> messageHandler);

  /*
   * Convenient overloaded function for listening.
   * @param messageHandler The callback function when a message is received.
   */
  virtual void listen(std::function<void(cubiumClientSocket_t*)> messageHandler);

  /*
   * Setter function for the server socket.
   * @param s The new serverSocket to use.
   */
  void setServerSock(cubiumServerSocket_t* s) { serverSock = s; }

  /*
   * Prints the RoutingTable in a pretty way.
   */
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
  /* serverSock   This socket is used for communication if the communicator serves the LSM */
  cubiumServerSocket_t* serverSock;
  /* clientSock   This socket is used for communication if the communicator serves a component*/
  cubiumClientSocket_t* clientSock;

  /* routingTable The shared routing table */
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
