#ifndef SPA_DATA_H
#define SPA_DATA_H

#include <cstdint>
#include <climits>
#include <spa_message.h>
#include <messages/op_codes.h>

struct SpaData
{
  SpaData(
      LogicalAddress consumerAddress,
      LogicalAddress producerAddress,
      uint64_t payload
      ) : spaMessage(0,0,sizeof(payload), consumerAddress, producerAddress, 0, op_SPA_DATA),
          dialogId(0),
          sequenceIndex(0),
          sequenceCount(0),
          interfaceId(0),
          messageId(0),
          payload(payload) {}
 
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
      uint64_t payload) : spaMessage(version, priority, sizeof(payload), consumerAddress, producerAddress, 0, op_SPA_DATA),
                   dialogId(dialogId),
                   sequenceIndex(sequenceIndex),
                   sequenceCount(sequenceCount),
                   interfaceId(interfaceId),
                   messageId(messageId),
                   payload(payload) {}

  SpaMessage spaMessage;
  uint16_t payloadLength;
  uint16_t dialogId;
  uint16_t sequenceIndex;
  uint16_t sequenceCount;
  uint8_t interfaceId;
  uint8_t messageId;
  uint64_t payload;
};
#endif
