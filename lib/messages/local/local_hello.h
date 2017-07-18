#ifndef LOCAL_HELLO_H
#define LOCAL_HELLO_H
#include <messages/op_codes.h>
#include <stdint.h>

#include <messages/local/local_spa_message.h>
struct LocalHello
{

  // CUUID UInt128 UUID Universally Unique Id of the SPA component
  // ComponentType UInt8 n/a The type of SPA component

  LocalHello(
      uint8_t version,
      uint8_t priority,
      LogicalAddress destination,
      LogicalAddress source,
      uint16_t flags,
      uint16_t sourcePort,
      uint64_t uuid,
      uint8_t componentType)
    : localSpaMessage(version,
                      priority,
                      72,
                      destination,
                      source,
                      flags,
                      op_LOCAL_HELLO,
                      sourcePort),
      uuid(uuid),
      componentType(componentType) {}

  LocalSpaMessage localSpaMessage;
  uint64_t uuid;
  // uint64_t uuidBack; // NOTE c++ standard does not have a 128 bit width integer
  uint8_t componentType;
};
#endif
