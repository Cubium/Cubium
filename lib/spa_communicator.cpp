#include <iostream>
#include <functional>
#include <memory>
#include "spa_communicator.hpp"

SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress) : currentAddress(currentAddress) {}
SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress, std::vector<std::shared_ptr<PhysicalCommunicator>> comms)
  : currentAddress(currentAddress) { addCommunicators(comms); }

void SpaCommunicator::addCommunicators(std::vector<std::shared_ptr<PhysicalCommunicator>> comms)
{
  communicators.insert(communicators.end(), comms.begin(), comms.end());
}

void SpaCommunicator::handleFailure()
{
  std::cout << "Spa Communicator Failed" << '\n';
}

std::shared_ptr<PhysicalCommunicator> SpaCommunicator::selectCommunicator(
    LogicalAddress address,
    std::vector<std::shared_ptr<PhysicalCommunicator>> const &communicators)
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
  if (message == nullptr)
  {
    return false;
  }

  std::shared_ptr<PhysicalCommunicator> com = selectCommunicator(
      message->spaHeader.destination,
      communicators);

  if (com == nullptr)
  {
    handleFailure();
    return false;
  }
  if(!com->sendMsg(message)) std::cout << "DID NOT SEND" << std::endl;
  return true;
}

//TODO document
void SpaCommunicator::listen(std::function<void(cubiumServerSocket_t*)> messageHandler)
{
  std::shared_ptr<LocalCommunicator> com = getLocalCommunicator();
  if (com == nullptr)
  {
    handleFailure();
  }
  com->listen(messageHandler);
}
