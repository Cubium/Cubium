#include "component.hpp"
#include "logical_address.h"
#include "messages/op_codes.h"
#include "messages/spa/subscription_request.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "messages/spa/spa_data.h"
#include "messages/spa/subscription_reply.h"
#include "messages/spa/subscription_request.h"

void component_messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock)
{
  if (sock->isBuf)
  {
    std::cout << "It is a buf!" << std::endl;
    comp->receiveBuffer(sock);
  }
  else
  {
    SpaMessage* message = (SpaMessage*)sock->buf;
    comp->receiveMessage(message);
  }
}

void Component::receiveBuffer(cubiumClientSocket_t* sock)
{
  SpaData<std::string> fauxMessage(lastCourier->spaMessage.spaHeader.destination, lastCourier->spaMessage.spaHeader.source, sock->buf);
  handleSpaData((SpaMessage*)&fauxMessage);
}

void Component::registerSubscriptionRequest(SpaMessage* message)
{
  SubscriptionReply reply(message->spaHeader.source, address);
  communicator->send((SpaMessage*)&reply);

  if (addSubscriber(message->spaHeader.source, 0))
  {
    std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
  }
  else
  {
    std::cout << "Failed to add subscriber\n";
  }

  publish();
}

void Component::handleSubscriptionReply(SpaMessage* message)
{
  // TODO Do we need this function?
}

void Component::subscribe(
    LogicalAddress producer,
    uint8_t priority,
    uint32_t leasePeriod,
    uint16_t deliveryRateDivisor)
{
  SubscriptionRequest request(
      0,                    // Version
      0,                    // Message priority
      producer,             // Address of the producer component
      address,              // Address of the consumer component
      subnetManagerAddress, // Address of the subscriptions manager component
      0,                    // Flags
      leasePeriod,          // Duration of the subscription
      dialogId,             // Dialog identifier sent by requester
      deliveryRateDivisor,  // Subscribe to every nth message
      0,                    // xTEDS interface ID
      0,                    // xTEDS message Id
      priority,             // Subscription priority
      0                     // Message type (0 = subscription, 1 = unsubscribtion)
      );

  communicator->getLocalCommunicator()->initSubDialogue((SpaMessage*)&request, sizeof(request),
                                                        [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });

  //++dialogId;
}

bool Component::addSubscriber(LogicalAddress la, uint16_t d)
{
  // TODO Check for duplicate
  //auto sub = std::find(subscribers.begin(), subscribers.end(), newSubscriber);
  // if (subscribers.end() == sub )
  // {
  //   subscribers.push_back(newSubscriber);
  //   return true;
  // }
  // else
  //   return false;

  {
    std::lock_guard<std::mutex> lock(m_subscribers);
    subscribers.emplace_back(la, d);
  }

  return true;
}

void Component::receiveMessage(SpaMessage* message)
{
  std::cout << "Received SpaMessage with opcode: " << (int)message->spaHeader.opcode << '\n';

  if (message == nullptr)
  {
    std::cout << "Tried receiving a nullptr.\n";
    return;
  }

  switch (message->spaHeader.opcode)
  {
  case op_LOCAL_ACK:
    return;

  case op_SPA_SUBSCRIPTION_REQUEST:
    registerSubscriptionRequest(message);
    return;

  case op_SPA_SUBSCRIPTION_REPLY:
    handleSubscriptionReply(message);
    return;

  case op_SPA_DATA:
    handleSpaData(message);
    return;

  case op_SPA_COURIER:
    lastCourier = (SpaCourier*)message;
    return;

  default:
    std::cout << "Unrecognized message" << std::endl;
    return;
  }
}

void Component::publish()
{
  /*
  ++comp->publishIter;

  if (comp->publishIter == 201)
  { // Max deliveryRateDivisor is therefore 200
    comp->publishIter = 1;
  }
  */

  /* send data */

  auto newThread = std::thread([&]() {
    for (;;)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
      std::lock_guard<std::mutex> lock(m_subscribers);
      for (auto i = 0u; i < subscribers.size(); ++i)
      {
        if (subscribers[i].deliveryRateDivisor % publishIter == 0)
        {
          sendData(subscribers[i].subscriberAddress);
        }
      }
    }
  });

  /* listen for more requests */
  communicator->getLocalCommunicator()->clientListen(
      [=](cubiumClientSocket_t* s) {
        component_messageCallback(shared_from_this(), s);
      });

  newThread.join();
}
