#ifndef LOCAL_ACK_H
#define LOCAL_ACK_H

#include <messages/local/local_spa_message.h>
#include <messages/op_codes.h>
#include <stdint.h>

struct LocalAck
{
  LocalAck(
      uint8_t version,
      uint8_t priority,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags,
      uint16_t sourcePort,
      uint8_t status) : localSpaMessage(version,
                                        priority,
                                        72,
                                        destination,
                                        source,
                                        flags,
                                        op_LOCAL_ACK,
                                        sourcePort),
                        status(status) {}
  LocalSpaMessage localSpaMessage;

  uint8_t status; // The status of the acknowledgement
};
#endif
