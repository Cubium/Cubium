#include "local_subnet_manager.hpp"
#include "logical_address.h"
#include "messages/local/local_ack.h"
#include "messages/op_codes.h"
#include "spa_message.h"
#include <memory>
#include <iostream>

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock)
{
  SpaMessage* msg = (SpaMessage*)sock->buf;

  auto op = msg->spaHeader.opcode;
  std::cout << "Received SpaMessage with opcode " << (int)op << " on port " << (int)sock->from.sin_port << "\n";
  if (op == op_LOCAL_HELLO)
  {
    lsm->components.add(msg->spaHeader.source);
    lsm->routingTable->insert(msg->spaHeader.source, sock->from.sin_port);

    LocalAck reply(0, 0, msg->spaHeader.source, LogicalAddress(1, 0), 0, 3500, 0);

    lsm->communicator->getLocalCommunicator()->serverSend((SpaMessage*)&reply, sizeof(reply));
  }
}
