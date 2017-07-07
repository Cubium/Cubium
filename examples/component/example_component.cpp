#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <messages/local/local_hello.h>
#include <local_component_routing_table.hpp>

class ExampleComponent : public Component
{
public:
  ExampleComponent(Component::Com com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*){}
  virtual void sendSpaData(LogicalAddress){}


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

	// While !ack, spam send message, once message is received. Send spa data.
    sendMsg((SpaMessage*)message);
  }

  static void messageCallback(void *buff, uint32_t len)
  {
	SpaMessage* message = (SpaMessage*)buff; 
	std::cout << "Opksdjfcode: " << (int)message->spaHeader.opcode << '\n';
	return;
  }
};

int main()
{
  std::shared_ptr<ServerSocket> sock = std::make_shared<ServerSocket>();
  auto routingTable = std::make_shared<LocalComponentRoutingTable>();

  LogicalAddress localAddress(1, 0);
  std::vector<SpaCommunicator::Com> comms = {
      std::make_shared<LocalCommunicator>(sock, routingTable, localAddress)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  ExampleComponent comp(spaCom);
  comp.appInit();
  std::cout << "Listening..." << std::endl;
  comp.communicator->listen(ExampleComponent::messageCallback);

  return 0;
}
