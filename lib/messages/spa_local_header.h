#ifndef SPA_LOCAL_HEADER_H
#define SPA_LOCAL_HEADER_H

#include <stdint.h>

struct SpaLocalHeader
{

  SpaLocalHeader(
      uint16_t sourcePort,
      uint16_t length,
      uint8_t opcode) : sourcePort(sourcePort), length(length), opcode(opcode) {}

  // SourcePort UInt16 n/a Port of the sending component
  // Length UInt16 n/a Length of the message payload
  // Opcode UInt8 n/a Unique opcode of the message
  uint16_t sourcePort;
  uint16_t length;
  uint8_t opcode;
};
#endif
