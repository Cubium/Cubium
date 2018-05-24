#include "../addresses.hpp"
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

  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>(la_LSM, sock);

  auto communicator = std::make_shared<LocalCommunicator>(&sock, routingTable, la_LSM);

  auto manager = std::make_shared<LocalSubnetManager>(communicator, routingTable, NUM_COMPONENTS, NUM_SUBSCRIPTIONS, la_LSM);
  manager->start();

  return 0;
}
