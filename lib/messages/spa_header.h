#ifndef SPA_HEADER_H
#define SPA_HEADER_H
struct SpaHeader
{
  SpaHeader(
      uint8_t const version = 0,
      uint8_t const priority = 0,
      uint16_t const length = 0,
      LogicalAddress const destination = LogicalAddress(0, 0),
      LogicalAddress const source = LogicalAddress(0, 0),
      uint16_t const flags = 0,
      uint8_t const opcode = 0)
    : version(version),
      priority(priority),
      length(length),
      destination(destination),
      source(source),
      flags(flags),
      opcode(opcode) {}

  SpaHeader(LogicalAddress const destination, uint8_t const opcode)
    : destination(destination),
      opcode(opcode),
      version(0),
      priority(0),
      length(0),
      source(LogicalAddress(0,0)),
      flags(0) {}

  uint8_t const version;
  uint8_t const priority;
  uint16_t const length;
  LogicalAddress const destination;
  LogicalAddress const source;
  uint16_t const flags;
  uint8_t const opcode;
};
#endif
