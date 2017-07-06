#ifndef SPA_HEADER_H
#define SPA_HEADER_H
struct SpaHeader
{

  SpaHeader(
      uint8_t version = 0,
      uint8_t priority = 0,
      uint16_t length = 0,
      LogicalAddress destination = LogicalAddress(0, 0),
      LogicalAddress source = LogicalAddress(0, 0),
      uint16_t flags = 0,
      uint8_t opcode = 0)
    : version(version),
      priority(priority),
      length(length),
      destination(destination),
      source(source),
      flags(flags),
      opcode(opcode) {}

  SpaHeader(LogicalAddress destination, uint8_t opcode)
    : destination(destination), opcode(opcode)
  {
    uint8_t version = 0;
    uint8_t priority = 0;
    uint16_t length = 0;
    LogicalAddress source = LogicalAddress(0, 0);
    uint16_t flags = 0;
  }

  uint8_t version;
  uint8_t priority;
  uint16_t length;
  LogicalAddress destination;
  LogicalAddress source;
  uint16_t flags;
  uint8_t opcode;
};
#endif
