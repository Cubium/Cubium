#include "local_communicator.hpp"

bool LocalCommunicator::sendMsg(SpaMessage * message, ssize_t len)
{
  auto socket = routingTable->getPhysicalAddress(message->spaHeader.destination);  
  serverSend(message, len);
}

void LocalCommunicator::handleFailure(std::string message)
{
  std::cout << "Local Communicator failure: " << message << '\n';
}

bool LocalCommunicator::serverSend(SpaMessage* message, ssize_t len)
{
  if (message == nullptr)
  {
    return false;
  }

  //routingTable->getPhysicalAddress(message->spaHeader.destination);

  serverSocket_send((void*)message, len, serverSock);
  return true;
}

bool LocalCommunicator::clientSend(SpaMessage* message, ssize_t len)
{
  if (message == nullptr)
  {
    return false;
  }

  clientSocket_send((void*)message, len, clientSock);
  return true;
}

void LocalCommunicator::clientListen(std::function<void(cubiumClientSocket_t*)> func)
{
  clientSocket_listen(clientSock, func);

}

void LocalCommunicator::listen(std::function<void(cubiumServerSocket_t*)> messageHandler)
{
  if (serverSock == nullptr)
  {
    handleFailure("serverSock == nullptr");
    return;
  }
  serverSocket_listen(serverSock, messageHandler);
}

void LocalCommunicator::listen(std::function<void(cubiumClientSocket_t*)> messageHandler)
{
  if (clientSock == nullptr)
  {
    handleFailure("clientSock == nullptr");
    return;
  }
  clientSocket_listen(clientSock, messageHandler);
}

void LocalCommunicator::clientConnect(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback)
{
  if (message == nullptr)
  {
    handleFailure("clientConnect: SpaMessage is a nullptr");
    return;
  }
  clientSocket_serverConnect(clientSock, (void*)message, len, callback);
}

/*
void LocalCommunicator::insertToRoutingTable(LogicalAddress log, cubium port)
{
  routingTable->insert(log, port);
}
*/
