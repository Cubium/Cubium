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
#include <mutex>
#include <socket/clientSocket.hpp>
#include <thread>
#include <unistd.h>

class CompB : public Component
{
public:
  CompB(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, la_CB, la_LSM)
  {
  }

  virtual void handleSpaData(SpaMessage* message)
  {

    auto op = message->spaHeader.opcode;
    std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';

    if (op == op_SPA_SUBSCRIPTION_REQUEST)
    {
      SubscriptionReply reply(message->spaHeader.source, la_CB);
      communicator->send((SpaMessage*)&reply);
      if (addSubscriber(message->spaHeader.source, 0))
      {
        std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
      }
      publish();
    }
    else if (op == op_SPA_DATA)
    {
      auto castMessage = (SpaData*)message;
      std::cout << "Payload: " << castMessage->payload << std::endl;
    }
  }

  virtual void sendSpaData(LogicalAddress address)
  {
  }

  void init()
  {
    subscribe(la_CA);
  }

};

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_CB)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_CB, comms);

  auto comp = std::make_shared<CompB>(spaCom);
  comp->preInit();
  comp->init();
  comp->listen();

  return 0;
}
