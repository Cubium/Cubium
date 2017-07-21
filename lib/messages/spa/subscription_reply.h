#ifndef SPA_SUBSCRIPTION_REPLY
#define SPA_SUBSCRIPTION_REPLY

#include <messages/op_codes.h>
#include <spa_message.h>
#include <stdint.h>

struct SubscriptionReply
{

  SubscriptionReply(
      LogicalAddress destination,
      LogicalAddress source)
    : spaMessage(0, 0, 0,
                 destination,
                 source,
                 0, 0x47),
      dialogId(0),
      replyType(0) {}

  SubscriptionReply(
      uint8_t version,
      uint8_t priority,
      uint16_t length,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags,
      uint16_t dI,
      uint8_t rT)
    : spaMessage(version,
                 priority,
                 length,
                 destination,
                 source,
                 flags,
                 0x47),
      dialogId(dI),
      replyType(rT) {}

  SpaMessage spaMessage;
  uint16_t dialogId;
  uint8_t replyType;
};

#endif
