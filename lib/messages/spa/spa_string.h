#ifndef SPA_STRING
#define SPA_STRING

#include <messages/op_codes.h>
#include <spa_message.h>
#include <stdint.h>

struct SpaString
{
  SpaString(
      LogicalAddress destination,
      LogicalAddress source)
    : spaMessage(0, 0, 0,
                 destination,
                 source,
                 0, op_SPA_STRING) {}

  SpaMessage spaMessage;
  char st[256];

/*
  SpaString(
      uint8_t version,
      uint8_t priority,
      uint16_t length,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags)
    : spaMessage(version,
                 priority,
                 length,
                 destination,
                 source,
                 flags,
                 op_SPA_STRING) {}
                 */
};

#endif
