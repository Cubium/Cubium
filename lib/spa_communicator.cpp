#include <iostream>
#include <functional>
#include "spa_communicator.hpp"

SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress) : currentAddress(currentAddress) {}
SpaCommunicator::SpaCommunicator(LogicalAddress currentAddress, std::vector<Com> comms)
  : currentAddress(currentAddress) { addCommunicators(comms); }

void SpaCommunicator::addCommunicators(std::vector<SpaCommunicator::Com> comms)
{
  communicators.insert(communicators.end(), comms.begin(), comms.end());
}

void SpaCommunicator::handleFailure()
{
  std::cout << "Spa Communicator Failed" << '\n';
}

SpaCommunicator::Com SpaCommunicator::selectCommunicator(
    LogicalAddress address,
    std::vector<Com> const &communicators)
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

SpaCommunicator::Com SpaCommunicator::getLocalCommunicator()
{
  return selectCommunicator(
      LogicalAddress(LOCAL_SUBNET_ADDRESS, 0), communicators);
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
  else std::cout << "COM SENT" << std::endl;
  return true;
}

//TODO document
void SpaCommunicator::listen(std::function<void(void *, uint32_t)> messageHandler)
{
  SpaCommunicator::Com com = getLocalCommunicator();
  if (com == nullptr)
  {
    handleFailure();
  }
  com->listen(messageHandler);
}
