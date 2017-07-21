#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <local_component_routing_table.hpp>
#include <messages/local/local_hello.h>
#include <messages/spa/subscription_request.h>
#include <messages/spa/subscription_reply.h>
#include <socket/clientSocket.hpp>
#include "messages/op_codes.h"
#include "../demo_addresses.hpp"

class ComponentB;

void messageCallback(std::shared_ptr<ComponentB> comp, cubiumClientSocket_t* sock);



class ComponentB : public Component, public std::enable_shared_from_this<ComponentB>
{
public:
  ComponentB(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*) {}
  virtual void sendSpaData(LogicalAddress) {}

  virtual void appInit()
  {
    std::cout << "Component B initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_CB, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s){ messageCallback(ComponentB::shared_from_this(), s); });
    communicator->getLocalCommunicator()->clientListen(
      [=](cubiumClientSocket_t* s){ messageCallback(ComponentB::shared_from_this(), s); });

   // std::cout << "Sending message with opcode:\n";

  //  communicator->send((SpaMessage*)&request);
 
  }

};

void messageCallback(std::shared_ptr<ComponentB> comp, cubiumClientSocket_t* sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;
  auto op = message->spaHeader.opcode;
  std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';

  if (op == op_SPA_SUBSCRIPTION_REQUEST)
  {
    SubscriptionReply reply(message->spaHeader.source, la_CB);
    comp->communicator->getLocalCommunicator()->clientSend((SpaMessage*)&reply, sizeof(SubscriptionReply));
  }

  return;
}

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_CB)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_CB, comms);

  auto comp = std::make_shared<ComponentB>(spaCom);
  comp->appInit();

  return 0;
}
