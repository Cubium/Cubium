#ifndef SPA_SUBSCRIPTION_REQUEST
#define SPA_SUBSCRIPTION_REQUEST

#include <messages/op_codes.h>
#include <stdint.h>
#include <spa_message.h>

struct SubscriptionRequest
{
  SubscriptionRequest(
    LogicalAddress destination,
    LogicalAddress source,
    LogicalAddress mA)
  : spaMessage(0,0,0,
               destination,
               source,
               0, 0x46),
    producerAddr(source),
    consumerAddr(destination),
    managerAddr(mA),
    leasePeriod(0),
    dialogId(0),
    deliveryRateDivisor(0),
    interfaceId(0),
    messageId(0),
    subscriptionPriority(0),
    type(0) {}
  
  SubscriptionRequest(
    uint8_t version,
    uint8_t priority,
    LogicalAddress destination,
    LogicalAddress source,
    LogicalAddress mA,
    uint16_t flags,
    uint32_t lP,
    uint16_t dI,
    uint16_t dRD,
    uint8_t  iI,
    uint8_t  mI,
    uint8_t  sP,
    uint8_t  t)
  : spaMessage(version,
               priority,
               sizeof(SubscriptionRequest),
               destination,
               source,
               flags,
               0x46 ),
    producerAddr(source),
    consumerAddr(destination),
    managerAddr(mA),
    leasePeriod(lP),
    dialogId(dI),
    deliveryRateDivisor(dRD),
    interfaceId(iI),
    messageId(mI),
    subscriptionPriority(sP),
    type(t) {}

  SpaMessage spaMessage;
  LogicalAddress producerAddr;
  LogicalAddress consumerAddr;
  LogicalAddress managerAddr;
  uint32_t leasePeriod;
  uint16_t dialogId;
  uint16_t deliveryRateDivisor;
  uint8_t  interfaceId;
  uint8_t  messageId;
  uint8_t  subscriptionPriority;
  uint8_t  type;

};
#endif
