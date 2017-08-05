#include "../demo_addresses.hpp"
#include "messages/op_codes.h"
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
#include "readLightPot.h"

//#define LIGHT_VERBOSE
#define LIVE_GRAPHS

class LightSensor;

void messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);


class LightSensor : public Component
{
public:
  LightSensor(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage* message)
  {
    auto op = message->spaHeader.opcode;
#ifdef LIGHT_VERBOSE
    std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';
#endif
    if (op == op_SPA_SUBSCRIPTION_REQUEST)
    {
      SubscriptionReply reply(message->spaHeader.source, la_light);
      communicator->send((SpaMessage*)&reply);
      if (addSubscriber(message->spaHeader.source, 0))
      {
#ifdef LIGHT_VERBOSE
        std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
#endif
      }
      publish();
    }
  }

  virtual void sendSpaData(LogicalAddress address)
  {
    float rawVal = readLightPot();
    float payload = rawVal * 15.151515;
#ifdef LIGHT_VERBOSE
    std::cout << "Sending SpaData: " << payload << std::endl;
#endif

#ifdef LIVE_GRAPHS_LIGHT
    std::cout << "0:" << payload << std::endl;
#endif

    SpaData dataMessage(address, la_light, payload);
    communicator->send((SpaMessage*)&dataMessage);
  }

  virtual void appInit()
  {
#ifdef LIGHT_VERBOSE
    std::cout << "Light sensor initializing!" << '\n';
#endif

    LocalHello hello(0, 0, la_LSM, la_light, 0, 0, 0, 0);

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

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_light)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_light, comms);

  auto comp = std::make_shared<LightSensor>(spaCom);
  comp->appInit();

  return 0;
}
