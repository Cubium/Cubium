#include <iostream>
#include <local_communicator.hpp>
#include <socket/serverSocket.hpp>
#include <subnet_manager.hpp>
#include <messages/local/local_spa_message.h>
#include <messages/local/local_ack.h>
#include <messages/op_codes.h>

int main(void)
{
  uint16_t port = 3500;

  cubiumServerSocket_t sock = serverSocket_openSocket(port);

  LogicalAddress localAddress(1,0);

  auto routingTable = std::make_shared<RoutingTable>(localAddress, port);

  std::vector<SpaCommunicator::Com> comms = {
	  std::make_shared<LocalCommunicator>(&sock, routingTable, localAddress)};

  auto spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  SubnetManager manager(spaCom, localAddress, port);
  manager.listenMessages();

  return 0;
}
