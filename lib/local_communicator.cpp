#include "local_communicator.hpp"

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

  if (routingTable->getPhysicalAddress(message->spaHeader.destination) < 0)
  {
    handleFailure("Address not in routing table");
    return false;
  }

  serverSocket_send((void*)message, len, serverSock);
  return true;
}

bool LocalCommunicator::clientSend(SpaMessage* message)
{
  if (message == nullptr)
  {
    return false;
  }

  clientSocket_send((void*)message, sizeof(SpaMessage*), clientSock);
  return true;
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
