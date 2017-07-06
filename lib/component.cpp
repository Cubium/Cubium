#include "component.hpp"
#include "logical_address.h"
//#include "messages/spa_subscription_reply.h"
//#include "messages/spa_subscription_request.h"
//#include "messages/spa_data.h"
#include "messages/op_codes.h"
#include <iostream>
#include <memory>

void Component::registerSubscriptionRequest(SpaMessage* message)
{
  /*
  if (message == nullptr)
  {
    std::cout << "Invalid subscription request" << std::endl;
  }

  auto castMessage = (SpaSubscriptionRequest*)(message);

  // Dynamic memory allocation !!!
  subscribers.emplace_back(castMessage->consumerAddress, castMessage->deliveryRateDivisor); 

  auto reply = new SpaSubscriptionReply(
      0,                            // Version
      0,                            // Message priority
      address,                      // Address of the producer component
      castMessage->consumerAddress, // Address of the consumer component
      castMessage->dialogId,        // Dialog identifer sent by requester
      0                             // 0 = accepted
      );

  sendMsg(reply);
  */
}

void Component::handleSubscriptionReply(SpaMessage* message)
{
  /*
  if (message == nullptr)
    return;
  auto castMessage = (SpaSubscriptionReply*)(message);

  // TODO actually implement subscription handling
  if (castMessage->replyType != 0)
  {
    std::cout << "SUBSCRIPTION FAILED" << std::endl;
  }
  else
  {
    std::cout << "Subscription succeeded." << std::endl;
  }
  */
}

void Component::subscribe(
    LogicalAddress producer,
    uint8_t priority,
    uint32_t leasePeriod,
    uint16_t deliveryRateDivisor)
{
  /*
  auto request = new SpaSubscriptionRequest(
      0,                    // Version
      0,                    // Message priority TODO 
      producer,             // Address of the producer component
      address,              // Address of the consumer component
      LogicalAddress(0, 0), // Address of the subscriptions manager component TODO
      leasePeriod,          // Duration of the subscription TODO
      dialogId,             // Dialog identifier sent by requester
      deliveryRateDivisor,  // Subscribe to every nth message
      0,                    // xTEDS interface ID TODO
      0,                    // xTEDS message Id TODO
      priority,             // Subscription priority TODO
      0                     // Message type (0 = subscription, 1 = unsubscribtion)
      );

  sendMsg(request);
  ++dialogId;
  */
}

void Component::receiveMessage(SpaMessage* message)
{
  if (message == nullptr)
  {
    return;
  }

  switch (message->spaHeader.opcode)
  {
    case op_SPA_SUBSCRIPTION_REQUEST:
      registerSubscriptionRequest(message);
      break;

    case op_SPA_SUBSCRIPTION_REPLY:
      handleSubscriptionReply(message);
      break;

    case op_SPA_DATA:
      handleSpaData(message);
      break;

    default:
      std::cout << "Unrecognized message" << std::endl;
      break;
  }
}

void Component::publish()
{
  for (auto i = 0u; i < subscribers.size(); ++i)
  {
    if (subscribers[i].deliveryRateDivisor % publishIter == 0)
    {
      sendSpaData(subscribers[i].subscriberAddress);
    }
  }

  ++publishIter;

  if (publishIter == 201)
  { // Max deliveryRateDivisor is therefore 200
    publishIter = 1;
  }
}
