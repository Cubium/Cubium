#include "../demo_addresses.hpp"
#include "messages/op_codes.h"
#include "random.hpp"
#include <chrono>
#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <local_component_routing_table.hpp>
#include <messages/local/local_hello.h>
#include <messages/spa/spa_data.h>
#include <messages/spa/subscription_reply.h>
#include <messages/spa/subscription_request.h>
#include <socket/clientSocket.hpp>
#include <thread>
#include <unistd.h>

class ComponentB;

void messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);

uint32_t readData();

class ComponentB : public Component
{
public:
  ComponentB(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage* message)
  {
    auto op = message->spaHeader.opcode;
    std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';
    if (op == op_SPA_SUBSCRIPTION_REQUEST)
    {
      SubscriptionReply reply(message->spaHeader.source, la_CB);
      communicator->getLocalCommunicator()->clientSend((SpaMessage*)&reply, sizeof(SubscriptionReply));
      if (addSubscriber(message->spaHeader.source, 0))
      {
        std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
      }
      publish();
    }
  }
  virtual void sendSpaData(LogicalAddress address)
  {
    SpaData dataMessage(address, la_CB, rand() % 100);
    communicator->getLocalCommunicator()->clientSend((SpaMessage*)&dataMessage, sizeof(SpaData));
  }

  virtual void appInit()
  {
    std::cout << "Component B initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_CB, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s) { messageCallback(shared_from_this(), s); });

    communicator->getLocalCommunicator()->clientListen(
        [=](cubiumClientSocket_t* s) { messageCallback(shared_from_this(), s); });
  }
};

void messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;
  comp->handleSpaData(message);
  return;
}

uint32_t readData()
{
  static int data = 0;
  int random = rand(1, 100);
  if (random < 5)
  {
    ++data;
    return rand(1,100);
  }
  return ++data;
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
