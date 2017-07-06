#include <iostream>
#include <local_communicator.hpp>
#include <subnet_manager.hpp>
#include <messages/local/local_spa_message.hpp>

int main(void)
{
/*
  std::shared_ptr<ServerSocket> sock = std::make_shared<ServerSocket>();
  sock->bindSocket(port);

  std::shared_ptr<RoutingTable> routingTable = std::make_shared<RoutingTable>();

  LogicalAddress localAddress(1, 0);
  std::vector<SpaCommunicator::Com> comms = {
      std::make_shared<LocalCommunicator>(sock, routingTable, localAddress)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);
  SubnetManager manager(spaCom);
  manager.listenMessages();
*/

  uint16_t port = 8888;

  std::shared_ptr<ServerSocket> sock = std::make_shared<ServerSocket>();
  sock->bindSocket(port);

  LogicalAddress localAddress(1,0);

  auto routingTable = std::make_shared<RoutingTable>(localAddress, port);

  std::vector<SpaCommunicator::Com> comms = {
	  std::make_shared<LocalCommunicator>(sock, routingTable, localAddress)};

  auto spaCom = std::make_shared<SpaCommunicator>(localAddress, comms);

  SubnetManager manager(spaCom, localAddress, port);
  manager.listenMessages();



  return 0;
}
