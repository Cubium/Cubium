#include <iostream>
#include <spa_message.h>
#include <subnet_manager.hpp>
#include <messages/op_codes.h>
#include <messages/local/local_ack.h>
#include <memory>
#include <messages/local/local_spa_message.h>
#include <messages/local/local_hello.h>

std::shared_ptr<SpaCommunicator> SubnetManager::communicator;

void SubnetManager::SM_messageCallback(cubiumServerSocket_t * sock)
{
    SpaMessage* msg = (SpaMessage*)sock->buf;

    auto op = msg->spaHeader.opcode;
    std::cout << "Received SpaMessage with opcode: " << (int)op << "\n";

    if (op == op_LOCAL_HELLO)
    {
      LocalAck reply(0, 0, msg->spaHeader.source, LogicalAddress(1,0), 0, 3500, 0);   

    //  communicator->getLocalCommunicator()->serverSend((SpaMessage*)&reply);
      serverSocket_send((void*)&reply, sizeof(reply), sock);
    }
}

