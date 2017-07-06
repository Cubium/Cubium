#include "local_communicator.hpp"

void LocalCommunicator::handleFailure()
{
  //TODO how should this be handled?
  std::cout << "Local Communicator failure" << '\n';
}

bool LocalCommunicator::sendMsg(std::shared_ptr<SpaMessage> message)
{
  if (message == nullptr)
  {
    return false;
  }

  uint16_t port = -1;

  //routing table never init, must be a component, send to default subnet port.
  if (routingTable == nullptr)
  {
	port = 8888;	  
  }	  
  
  else
  {
   port = routingTable->getPhysicalAddress(message->spaHeader.destination);
  }
  if (port < 0)
  {
	std::cout << port << std::endl;
    handleFailure();
    return false;
  }


  //Nick plz, Marshall returns the length of the message, and puts the message into buff. But somehow buff still has error.
  uint8_t *buff = nullptr;
  uint32_t buffLen = message->marshal(buff);
  sock->send(SERVER, port, buff, buffLen);
  return true;
}

void LocalCommunicator::listen(std::function<void(uint8_t *, uint32_t)> messageHandler)
{
  if (sock == nullptr)
  {
    handleFailure();
    return;
  }
  sock->listen(messageHandler);
}

void LocalCommunicator::insertToRoutingTable(LogicalAddress log, uint32_t port)
{
  routingTable->insert(log,port);
}
