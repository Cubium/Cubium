#include "local_communicator.hpp"

void LocalCommunicator::handleFailure()
{
  //TODO how should this be handled?
  std::cout << "Local Communicator failure" << '\n';
}

bool LocalCommunicator::sendMsg(SpaMessage* message)
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


  printf("Opcode before sending: %d\n", message->spaHeader.opcode);
  //Nick plz, Marshall returns the length of the message, and puts the message into buff. But somehow buff still has error.
  //void* buff = (void*)message;
  size_t buffLen = sizeof(message);
  sock->send(SERVER, port, (void*)message, buffLen);
  return true;
}

void LocalCommunicator::listen(std::function<void(void *, uint32_t)> messageHandler)
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
