#include "local_subnet_manager.hpp"
#include "logical_address.h"
#include "spa_message.h"
#include "messages/op_codes.h"
#include "messages/local/local_ack.h"
#include <memory>

void LocalSubnetManager::receiveMessage(SpaMessage* message)
{
  if (message->spaHeader.opcode == op_LOCAL_HELLO)
  {
    components.add(message->spaHeader.source);
  }
}
