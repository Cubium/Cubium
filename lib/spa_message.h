#ifndef SPA_MESSAGE_H
#define SPA_MESSAGE_H
#include <memory>
#include <stdint.h>

#include "logical_address.h"
#include "messages/spa_header.h"

//! Class to represent generic spa message. Should be extended by other Spa Message
//! types
struct SpaMessage
{

  SpaMessage(LogicalAddress destination, uint8_t opcode)
    : spaHeader(destination, opcode) {}

  SpaMessage(
      uint8_t version = 0,
      uint8_t priority = 0,
      uint16_t length = 0,
      LogicalAddress destination = LogicalAddress(0, 0),
      LogicalAddress source = LogicalAddress(0, 0),
      uint16_t flags = 0,
      uint8_t opcode = 0) : spaHeader(version,
                                      priority,
                                      length,
                                      destination,
                                      source,
                                      flags,
                                      opcode) {}

  virtual ~SpaMessage() {}

  SpaHeader spaHeader;
};

#endif
