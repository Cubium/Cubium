#include <iostream>
#include <local_communicator.hpp>
#include <socket/serverSocket.hpp>
#include <subnet_manager.hpp>
#include <messages/local/local_spa_message.h>
#include <messages/local/local_ack.h>
#include <stdint.h>

void messageCallback(cubiumServerSocket_t * sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;

  std::cout << "Opcode:" << message->spaHeader.opcode << "\n";

  LocalAck * reply = new LocalAck(0,0,message->spaHeader.source,LogicalAddress(1,0),0,3500,0);
  serverSocket_send((void*)reply, sizeof(LocalAck*), sock);
}

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
  manager.listenMessages(messageCallback);

  return 0;
}
