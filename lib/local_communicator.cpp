#include "local_communicator.hpp"
#include "messages/op_codes.h"
#include "messages/spa/spa_data.h"

bool LocalCommunicator::sendMsg(SpaMessage* message, ssize_t len)
{
  if (nullptr == serverSock) // If communicator services a client
  {
    return clientSend(message, len);
  }
  else
  {
    return serverSend(message, len);
  }
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

void LocalCommunicator::clientListen(std::function<void(cubiumClientSocket_t*)> func, uint8_t exitOp)
{
  clientSocket_listen(clientSock, func, exitOp);
}

void LocalCommunicator::listen(std::function<int(cubiumServerSocket_t*)> messageHandler)
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
  clientSocket_listen(clientSock, messageHandler, 0);
}

void LocalCommunicator::clientConnect(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback)
{
  if (message == nullptr)
  {
    handleFailure("clientConnect: SpaMessage is a nullptr");
    return;
  }
  clientSocket_requestDialogue(clientSock, (void*)message, len, callback, op_LOCAL_ACK);
}

void LocalCommunicator::initSubDialogue(SpaMessage* message, size_t len, std::function<void(cubiumClientSocket_t*)> callback)
{
  if (message == nullptr)
  {
    handleFailure("initSubDialogue: SpaMessage is a nullptr");
    return;
  }
  clientSocket_requestDialogue(clientSock, (void*)message, len, callback, op_SPA_SUBSCRIPTION_REPLY);
}
