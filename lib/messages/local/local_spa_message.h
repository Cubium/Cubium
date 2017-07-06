#ifndef LOCAL_SPA_MESSAGE
#define LOCAL_SPA_MESSAGE
#include <cstdint>

#include <messages/spa_local_header.h>
#include <spa_message.h>

struct LocalSpaMessage
{
  LocalSpaMessage(
      uint8_t version,
      uint8_t priority,
      uint16_t length,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags,
      uint8_t opcode,
      uint16_t sourcePort) : spaMessage(version, priority, length, destination, source, flags, opcode),
                             spaLocalHeader(sourcePort, length, opcode) {}

  SpaMessage spaMessage;
  SpaLocalHeader spaLocalHeader;
};
#endif
