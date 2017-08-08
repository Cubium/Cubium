#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "spa_communicator.hpp"
#include "spa_message.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include "messages/local/local_hello.h"

struct Subscriber
{
  Subscriber(LogicalAddress la, uint16_t d)
    : subscriberAddress(la), deliveryRateDivisor(d) {}
  LogicalAddress subscriberAddress;
  uint16_t deliveryRateDivisor;
};

class Component;
void component_messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);

class Component : public std::enable_shared_from_this<Component>
{
public:
  Component(std::shared_ptr<SpaCommunicator> communicator = nullptr, LogicalAddress address = LogicalAddress(0, 0), LogicalAddress subnetManagerAddress = LogicalAddress(0,0))
    : communicator(communicator),
      address(address),
      subnetManagerAddress(subnetManagerAddress),
      dialogId(0),
      publishIter(1)

  {
    subscribers.reserve(8); // Default to 8 subscribers

    std::cout << "Component initializing!" << '\n';
 }

  virtual ~Component() {}
  //virtual void appShutdown() = 0;

  void publish();

  virtual void sendSpaData(LogicalAddress) = 0;

  virtual void handleSpaData(SpaMessage*) = 0;
  virtual void preInit()
  {
    LocalHello hello(0, 0, subnetManagerAddress, address, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });
  }

  virtual void listen()
  {
    communicator->getLocalCommunicator()->clientListen(
        [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });
  }
 
  virtual void init() = 0;

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

  void subscribe(LogicalAddress producer) { subscribe(producer, 0, 0, 0); }
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
  LogicalAddress subnetManagerAddress;
  uint8_t publishIter;
  uint16_t dialogId;
};

#endif
