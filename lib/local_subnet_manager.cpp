#include "local_subnet_manager.hpp"
#include "logical_address.h"
#include "messages/local/local_ack.h"
#include "messages/op_codes.h"
#include "spa_message.h"
#include <memory>

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t *sock)
{
  SpaMessage *msg = (SpaMessage *)sock->buf;

  auto op = msg->spaHeader.opcode;
  std::cout << "Received SpaMessage with opcode: " << (int)op << "\n";

  if (op == op_LOCAL_HELLO)
  {
    lsm->components.add(msg->spaHeader.source);

    LocalAck reply(0, 0, msg->spaHeader.source, LogicalAddress(1, 0), 0, 3500, 0);

    //  communicator->getLocalCommunicator()->serverSend((SpaMessage*)&reply);
    serverSocket_send((void *)&reply, sizeof(reply), sock);
  }
}
