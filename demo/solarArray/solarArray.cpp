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

class SolarArray;

void messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);


class SolarArray : public Component
{
public:
  SolarArray(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage* message)
  {
    auto op = message->spaHeader.opcode;
    std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';

    if (op == op_SPA_SUBSCRIPTION_REQUEST)
    {
      SubscriptionReply reply(message->spaHeader.source, la_solarArray);
      communicator->send((SpaMessage*)&reply);
      if (addSubscriber(message->spaHeader.source, 0))
      {
        std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
      }
      publish();
    }
    else if (op == op_SPA_DATA)
    {
      auto dataMessage = (SpaData*)message;
      std::cout << "Received data with payload: " << (int)dataMessage->payload << " from " << message->spaHeader.source << std::endl;
      if ((int)dataMessage->payload == 1)
      {
        system("python ~/Documents/cubium/demo/solarArray/panelDeploy.py");
        sleep(10);
      }
    }
  }

  virtual void sendSpaData(LogicalAddress address)
  {
    auto payload = rand() % 100;
    std::cout << "Sending SpaData: " << payload << std::endl;

    SpaData dataMessage(address, la_solarArray, payload);
    communicator->send((SpaMessage*)&dataMessage);
  }

  virtual void appInit()
  {
    std::cout << "Solar Array initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_solarArray, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s) { messageCallback(shared_from_this(), s); });

    SubscriptionRequest request1(la_medianFilter, la_solarArray, la_LSM);
    communicator->getLocalCommunicator()->initSubDialogue((SpaMessage*)&request1, sizeof(request1), [=](cubiumClientSocket_t* s) { messageCallback(shared_from_this(), s); });

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
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_solarArray)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_solarArray, comms);

  auto comp = std::make_shared<SolarArray>(spaCom);
  comp->appInit();

  return 0;
}
