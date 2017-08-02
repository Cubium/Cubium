#include "local_subnet_manager.hpp"
#include "logical_address.h"
#include "messages/local/local_ack.h"
#include "messages/op_codes.h"
#include "messages/spa/subscription_request.h"
#include "messages/spa/subscription_reply.h"
#include "messages/spa/spa_data.h"
#include "spa_message.h"
#include <memory>
#include <iostream>

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock)
{

  if (lsm->routingTable->isEmpty())
  {
    std::cout << "Nothing in the routing table." << std::endl;
  }

  SpaMessage* msg = (SpaMessage*)sock->buf;

  auto op = msg->spaHeader.opcode;
  std::cout << "Received SpaMessage with opcode " << (uint16_t)op << " on port " << (int)sock->from.sin_port << "\n";

  if (op == op_LOCAL_HELLO)
  {
    lsm->components.add(msg->spaHeader.source);
    lsm->routingTable->insert(msg->spaHeader.source, *sock);
    lsm->communicator->getLocalCommunicator()->printTable();

    LocalAck reply(0, 0, msg->spaHeader.source, LogicalAddress(1, 0), 0, 3500, 0);

    lsm->communicator->getLocalCommunicator()->serverSend((SpaMessage*)&reply, sizeof(reply));
  }
  else if (op == op_SPA_SUBSCRIPTION_REQUEST)
  {
    auto newSock = lsm->routingTable->getPhysicalAddress(msg->spaHeader.destination);
    serverSocket_send(msg, sizeof(SubscriptionRequest), &newSock);
  }
  else if (op == op_SPA_SUBSCRIPTION_REPLY)
  {
    auto newSock = lsm->routingTable->getPhysicalAddress(msg->spaHeader.destination);
    serverSocket_send(msg, sizeof(SubscriptionReply), &newSock);
  }
  else if (op == op_SPA_DATA)
  {
    auto newmsg = (SpaData*)sock->buf;
    std::cout << "LSMmessageCallback: " << newmsg->payload << std::endl;
    auto newSock = lsm->routingTable->getPhysicalAddress(msg->spaHeader.destination);
    serverSocket_send(msg, sizeof(SpaData), &newSock);
  }




}
