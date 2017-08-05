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
#include "readTempPot.h"

class TempSensor;

void messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);


class TempSensor : public Component
{
public:
  TempSensor(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage* message)
  {
    auto op = message->spaHeader.opcode;
    std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';

    if (op == op_SPA_SUBSCRIPTION_REQUEST)
    {
      SubscriptionReply reply(message->spaHeader.source, la_temp);
      communicator->send((SpaMessage*)&reply);
      if (addSubscriber(message->spaHeader.source, 0))
      {
        std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
      }
      publish();
    }
  }

  virtual void sendSpaData(LogicalAddress address)
  {
    
    auto payload = readTempPot();
    std::cout << "Sending SpaData: " << payload << std::endl;

    SpaData dataMessage(address, la_temp, payload);
    communicator->send((SpaMessage*)&dataMessage);
  }

  virtual void appInit()
  {
    std::cout << "Temperature sensor initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_temp, 0, 0, 0, 0);

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
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_temp)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_temp, comms);

  auto comp = std::make_shared<TempSensor>(spaCom);
  comp->appInit();

  return 0;
}