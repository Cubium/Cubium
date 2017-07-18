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

  //! Generate a message from a byte array

  //! \param serialized - array containing serialized message
  //! \param size - length of serialized array
  //SpaMessage(uint8_t *serialized, uint32_t size);

  //! Serializes a message to a byte array

  //! \param target - pointer where seialized message array will be stored
  //! \return - size of serialized array
  //uint32_t marshal(uint8_t *&target);

  //TODO document
  //static std::shared_ptr<SpaMessage> unmarshal(uint8_t *serialized, uint32_t size);

  //! Logical address of component on spa network
  //LogicalAddress logicalAddress; //NOTE deprecated

  SpaHeader spaHeader;
};

#endif
