#include <iostream>
#include <local_communicator.hpp>
#include <local_subnet_manager.hpp>
#include <memory>
#include <messages/local/local_ack.h>
#include <messages/local/local_spa_message.h>
#include <messages/op_codes.h>
#include <socket/serverSocket.hpp>

int main(void)
{
  uint16_t port = 3500;

  cubiumServerSocket_t sock = serverSocket_openSocket(port);

  LogicalAddress localAddress(1, 0);

  auto routingTable = std::make_shared<RoutingTable>(localAddress, port);

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, localAddress)};

  auto spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  auto manager = std::make_shared<LocalSubnetManager>(spaCom, localAddress, port);
  manager->listenMessages();

  return 0;
}
