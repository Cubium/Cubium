#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "spa_communicator.hpp"
#include "spa_message.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
//#include "messages/spa_data.h"
//#include "messages/spa_subscription_reply.h"
//#include "messages/spa_subscription_request.h"

struct Subscriber
{
  Subscriber(LogicalAddress la, uint16_t d)
    : subscriberAddress(la), deliveryRateDivisor(d) {}
  LogicalAddress subscriberAddress;
  uint16_t deliveryRateDivisor;
};

class Component
{
public:
  Component(std::shared_ptr<SpaCommunicator> communicator = nullptr, LogicalAddress address = LogicalAddress(0, 0))
    : communicator(communicator),
      address(address),
      dialogId(0),
      publishIter(1)

  {
    subscribers.reserve(8); // Default to 8 subscribers
  }

  virtual ~Component() {}
  //virtual void appShutdown() = 0;

  void publish();

  virtual void sendSpaData(LogicalAddress) = 0;

  virtual void handleSpaData(SpaMessage*) = 0;
  virtual void appInit() = 0;

  void sendMsg(SpaMessage* message, ssize_t len)
  {
    if (message == nullptr || communicator == nullptr)
    {
      return;
    }
    communicator->send(message, len);
  }

  void receiveMessage(SpaMessage*);

  void handleSubscriptionReply(SpaMessage*);
  void registerSubscriptionRequest(SpaMessage*);

  void subscribe(
      LogicalAddress producer,
      uint8_t priority,
      uint32_t leasePeriod,
      uint16_t deliveryRateDivisor);

  bool addSubscriber(LogicalAddress, uint16_t);

  std::shared_ptr<SpaCommunicator> communicator;
  std::vector<Subscriber> subscribers; // Should we make this a vector of pointers?
  std::mutex m_subscribers;

protected:
  LogicalAddress address;
  uint8_t publishIter;
  uint16_t dialogId;
};

#endif
