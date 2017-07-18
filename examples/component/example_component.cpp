#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <local_component_routing_table.hpp>
#include <messages/local/local_hello.h>
#include <socket/clientSocket.hpp>

class ExampleComponent : public Component
{
public:
  ExampleComponent(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage *) {}
  virtual void sendSpaData(LogicalAddress) {}

  static void messageCallback(cubiumClientSocket_t *sock)
  {
    SpaMessage *message = (SpaMessage *)sock->buf;
    std::cout << "Received SpaMessage with opcode: " << (int)message->spaHeader.opcode << '\n';
    return;
  }

  virtual void appInit()
  {
    std::cout << "Example component initializing!" << '\n';

    LocalHello hello(0, 0, LogicalAddress(0, 0), LogicalAddress(0, 0), 0, 8888, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage *)&hello, sizeof(hello), messageCallback);
  }
};

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<LocalComponentRoutingTable>();

  LogicalAddress localAddress(1, 0);
  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, localAddress)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  ExampleComponent comp(spaCom);
  comp.appInit();

  return 0;
}
