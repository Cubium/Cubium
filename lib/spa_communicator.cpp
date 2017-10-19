#include "spa_communicator.hpp"
#include "messages/local/local_ack.h"
#include "messages/local/local_hello.h"
#include "messages/op_codes.h"
#include "messages/spa/spa_data.h"
#include "messages/spa/spa_string.h"
#include "messages/spa/subscription_reply.h"
#include "messages/spa/subscription_request.h"
#include <functional>
#include <iostream>
#include <memory>

SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress) : currentAddress(currentAddress) {}
SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress, std::vector<std::shared_ptr<PhysicalCommunicator>> comms)
  : currentAddress(currentAddress) { addCommunicators(comms); }

void SpaCommunicator::addCommunicators(std::vector<std::shared_ptr<PhysicalCommunicator>> comms)
{
  communicators.insert(communicators.end(), comms.begin(), comms.end());
}

void SpaCommunicator::handleFailure(std::string message)
{
  std::cout << "Spa Communicator Failed: " << message << '\n';
}

std::shared_ptr<PhysicalCommunicator> SpaCommunicator::selectCommunicator(
    LogicalAddress address,
    std::vector<std::shared_ptr<PhysicalCommunicator>> const& communicators)
{
  for (auto com : communicators)
  {
    if (com == nullptr)
    {
      continue;
    }
    else if (com->getSubnetAddress().isOnSameSubnet(address))
    {
      return com;
    }
  }
  return nullptr;
}

std::shared_ptr<LocalCommunicator> SpaCommunicator::getLocalCommunicator()
{
  return std::dynamic_pointer_cast<LocalCommunicator>(selectCommunicator(
      LogicalAddress(LOCAL_SUBNET_ADDRESS, 0), communicators));
}

bool SpaCommunicator::send(SpaMessage* message)
{
  return send(0, message);
}

bool SpaCommunicator::send(ssize_t len, SpaMessage* message)
{
  switch (message->spaHeader.opcode)
  {
  case op_SPA_SUBSCRIPTION_REQUEST:
    send(message, sizeof(SubscriptionRequest));
    break;
  case op_SPA_SUBSCRIPTION_REPLY:
    send(message, sizeof(SubscriptionReply));
    break;
  case op_SPA_DATA:
    send(message, len);
    break;
  case op_SPA_STRING:
    send(message, sizeof(SpaString));
    break;
  case op_LOCAL_HELLO:
    send(message, sizeof(LocalHello));
    break;
  case op_LOCAL_ACK:
    send(message, sizeof(LocalAck));
    break;
  }
}

bool SpaCommunicator::send(SpaMessage* message, ssize_t len)
{
  if (message == nullptr)
  {
    return false;
  }

  std::shared_ptr<PhysicalCommunicator> com = selectCommunicator(
      message->spaHeader.destination,
      communicators);

  if (com == nullptr)
  {
    handleFailure("Communicator is a nullptr");
    return false;
  }
  else if (!com->sendMsg(message, len))
  {
    handleFailure("Message did not send");
    return false;
  }

  return true;
}

//TODO document
void SpaCommunicator::listen(std::function<void(cubiumServerSocket_t*)> messageHandler)
{
  std::shared_ptr<LocalCommunicator> com = getLocalCommunicator();
  if (com == nullptr)
  {
    handleFailure("(Listen - Server) Communicator is a nullptr");
  }
  com->listen(messageHandler);
}

void SpaCommunicator::listen(std::function<void(cubiumClientSocket_t*)> messageHandler)
{
  std::shared_ptr<LocalCommunicator> com = getLocalCommunicator();
  if (com == nullptr)
  {
    handleFailure("(Listen - Client) Communicator is a nullptr");
  }
  com->listen(messageHandler);
}
