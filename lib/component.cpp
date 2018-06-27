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

/*
 * See component.hpp for detailed documentation
 */

void component_messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;
  comp->receiveMessage(message);
}

void Component::registerSubscriptionRequest(SpaMessage* message)
{
  SubscriptionReply reply(message->spaHeader.source, address);
  communicator->sendMsg((SpaMessage*)&reply, sizeof(SubscriptionReply));

  if (addSubscriber(message->spaHeader.source, 0))
  {
    //std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
  }
  else
  {
    //std::cout << "Failed to add subscriber\n";
  }
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
      0,                    // Dialog identifier sent by requester
      deliveryRateDivisor,  // Subscribe to every nth message
      0,                    // xTEDS interface ID
      0,                    // xTEDS message Id
      priority,             // Subscription priority
      0                     // Message type (0 = subscription, 1 = unsubscribtion)
      );

  communicator->initSubDialogue((SpaMessage*)&request, sizeof(request),
                                [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });

  //++dialogId;
}

bool Component::addSubscriber(LogicalAddress la, uint16_t d)
{
  {
    std::lock_guard<std::mutex> lock(m_subscribers);
    subscribers.emplace_back(la, d);
  }
  return true;
}

void Component::receiveMessage(SpaMessage* message)
{
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
    checkForSubscriptionFailure(message);
    return;

  case op_SPA_DATA:
    handleSpaData(message);
    return;

  case op_SPA_STRING:
    handleSpaData(message);
    return;

  default:
    std::cout << "Unrecognized message" << std::endl;
    return;
  }
}

void Component::checkForSubscriptionFailure(SpaMessage* message)
{
  if (message->spaHeader.source == subnetManagerAddress)
  {
    std::cout << "Subscription failed" << std::endl;
  }
}


void Component::publish()
{

  /* Spin up a thread to handle the data-publishing loop */
  auto publishLoopThread = std::thread([&]() {
    for (;;)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
      std::lock_guard<std::mutex> lock(m_subscribers);
      for (auto i = 0u; i < subscribers.size(); ++i)
      {
        //        if (subscribers[i].deliveryRateDivisor % publishIter == 0)
        //        {
        sendDataThreaded(subscribers[i].subscriberAddress);
        //        }
      }
    }
  });

  /* Start up the listening service on the parent thread */
  // TODO I think we can just call listen() here
  communicator->clientListen(
      [=](cubiumClientSocket_t* s) {
        component_messageCallback(shared_from_this(), s);
      }, 0);

  publishLoopThread.join();
}

void Component::compSleep(int n)
{
  sleep(n);
}
