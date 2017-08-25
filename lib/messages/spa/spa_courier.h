#ifndef SPA_COURIER
#define SPA_COURIER

#include <messages/op_codes.h>
#include <spa_message.h>
#include <stdint.h>

struct SpaCourier
{
  SpaCourier(
      LogicalAddress destination,
      LogicalAddress source,
      ssize_t followerSize)
    : spaMessage(0, 0, 0,
                 destination,
                 source,
                 0, op_SPA_COURIER),
      followerSize(followerSize) {}

  SpaCourier(
      uint8_t version,
      uint8_t priority,
      uint16_t length,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags,
      ssize_t fS)
    : spaMessage(version,
                 priority,
                 length,
                 destination,
                 source,
                 flags,
                 op_SPA_COURIER),
      followerSize(fS) {}

  SpaMessage spaMessage;
  ssize_t followerSize;
};

#endif
