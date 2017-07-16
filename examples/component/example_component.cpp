#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <messages/local/local_hello.h>
#include <local_component_routing_table.hpp>
#include <socket/clientSocket.hpp>

class ExampleComponent : public Component
{
public:
  ExampleComponent(Component::Com com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*){}
  virtual void sendSpaData(LogicalAddress){}


  static void messageCallback(cubiumClientSocket_t * sock)
  {
    SpaMessage* message = (SpaMessage*)sock->buf; 
    std::cout << "Opcode: " << (int)message->spaHeader.opcode << '\n';
    return;
  }

  virtual void appInit()
  {
    std::cout << "Example app initializing!" << '\n';

    uint8_t version = 0;
    uint8_t priority = 0;
    LogicalAddress destination(1,0);
    LogicalAddress source(1,1);
    uint16_t flags = 0;
    uint16_t sourcePort = 8889;
    uint64_t uuid = 1;
    uint8_t componentType = 1;

    LocalHello* message = new LocalHello(
      version,
      priority,
      destination,
      source,
      flags,
      sourcePort,
      uuid,
      componentType
    );

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)message, messageCallback);
    
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
