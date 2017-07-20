#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <local_component_routing_table.hpp>
#include <messages/local/local_hello.h>
#include <messages/spa/subscription_request.h>
#include <socket/clientSocket.hpp>
#include "../demo_addresses.hpp"

void messageCallback(cubiumClientSocket_t* sock)
{
    SpaMessage* message = (SpaMessage*)sock->buf;
    std::cout << "Received SpaMessage with opcode: " << (int)message->spaHeader.opcode << '\n';
    return;
}


class ComponentB : public Component
{
public:
  ComponentB(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*) {}
  virtual void sendSpaData(LogicalAddress) {}

  virtual void appInit()
  {
    std::cout << "Component B initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_CB, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), messageCallback);
    communicator->getLocalCommunicator()->clientListen(messageCallback);

   // std::cout << "Sending message with opcode:\n";

  //  communicator->send((SpaMessage*)&request);
 
  }

};

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t*>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_CB)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_CB, comms);

  ComponentB comp(spaCom);
  comp.appInit();

  return 0;
}
