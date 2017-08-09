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

#define COMP_NAME CompA

class CompA : public Component
{
public:
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, la_CA, la_LSM) { }

  virtual void handleSpaData(SpaData* message)
  {
    std::cout << "Payload: " << message->payload << std::endl;
  }

  virtual void sendSpaData(LogicalAddress address)
  {
    sleep(1);
    auto payload = rand() % 100;

    std::cout << "Sending SpaData: " << payload << std::endl;

    SpaData dataMessage(address, la_CA, payload);
    communicator->send((SpaMessage*)&dataMessage);
  }

  void init()
  {
  }

};

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_CA)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_CA, comms);

  auto comp = std::make_shared<COMP_NAME>(spaCom);
  comp->preInit();
  comp->init();
  comp->listen();

  return 0;
}
