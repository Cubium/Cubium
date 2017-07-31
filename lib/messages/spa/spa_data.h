#ifndef SPA_DATA_H
#define SPA_DATA_H

#include <cstdint>
#include <climits>
#include <spa_message.h>
#include <messages/op_codes.h>

template <typename T>
struct SpaData
{
  SpaData(
      uint8_t version,
      uint8_t priority,
      LogicalAddress producerAddress,
      LogicalAddress consumerAddress,
      uint16_t dialogId,
      uint16_t sequenceIndex,
      uint16_t sequenceCount,
      uint8_t interfaceId,
      uint8_t messageId,
      T payload) : spaMessage(version, priority, 100 + payloadLength, consumerAddress, producerAddress, 0, op_SPA_DATA),
                   dialogId(dialogId),
                   sequenceIndex(sequenceIndex),
                   sequenceCount(sequenceCount),
                   interfaceId(interfaceId),
                   messageId(messageId),
                   payload(payload) {}

  uint16_t payloadLength = sizeof(payload) * CHAR_BIT;
  SpaMessage spaMessage;
  uint16_t dialogId;
  uint16_t sequenceIndex;
  uint16_t sequenceCount;
  uint8_t interfaceId;
  uint8_t messageId;
  T payload;
};
#endif
