#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <messages/local/local_hello.h>
#include <local_component_routing_table.hpp>
#include <socket/clientSocket.hpp>
#include <unistd.h>
#include <string.h>

class ExampleComponent : public Component
{
public:
  ExampleComponent(Component::Com com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*){}
  virtual void sendSpaData(LogicalAddress){}


  static void messageCallback(cubiumClientSocket_t * sock)
  {
    SpaMessage* message = (SpaMessage*)sock->buf; 
    std::cout << "Received SpaMessage with opcode: " << (int)message->spaHeader.opcode << '\n';
    return;
  }

  virtual void appInit()
  {
    std::cout << "Example component initializing!" << '\n';

    uint8_t version = 0;
    uint8_t priority = 0;
    LogicalAddress destination(1,0);
    LogicalAddress source(1,1);
    uint16_t flags = 0;
    uint16_t sourcePort = 8889;
    uint64_t uuid = 1;
    uint8_t componentType = 1;

    SpaMessage hello(LogicalAddress(0,0),0x20);
    
    communicator->getLocalCommunicator()->clientConnect(&hello, sizeof(hello), messageCallback);
    
  }


};

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<LocalComponentRoutingTable>();

  LogicalAddress localAddress(1, 0);
  std::vector<SpaCommunicator::Com> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, localAddress)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  ExampleComponent comp(spaCom);
  comp.appInit();

  return 0;
}
