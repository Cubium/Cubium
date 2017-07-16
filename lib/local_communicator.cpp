#include "local_communicator.hpp"

void LocalCommunicator::handleFailure()
{
  //TODO how should this be handled?
  std::cout << "Local Communicator failure" << '\n';
}

bool LocalCommunicator::serverSend(SpaMessage* message)
{
  if (message == nullptr)
  {
    return false;
  }

  if (routingTable->getPhysicalAddress(message->spaHeader.destination) < 0)
  {
    handleFailure();
    return false;
  }

  serverSocket_send((void*)message, sizeof(SpaMessage*), serverSock);
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

void LocalCommunicator::listen(std::function<void(cubiumServerSocket_t *)> messageHandler)
{
  if (serverSock == nullptr)
  {
    handleFailure();
    return;
  }
  serverSocket_listen(serverSock, messageHandler);
}

void LocalCommunicator::clientConnect(SpaMessage * message, std::function<void(cubiumClientSocket_t *)> callback)
{
  if (message == nullptr)
  {
    handleFailure();
    return;
  }
  clientSocket_serverConnect(clientSock, (void*)message, sizeof(SpaMessage*), callback);
}


void LocalCommunicator::insertToRoutingTable(LogicalAddress log, uint32_t port)
{
  routingTable->insert(log,port);
}
