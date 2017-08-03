#include "component.hpp"
#include "logical_address.h"
#include "messages/op_codes.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <thread>

//#include "messages/spa_data.h"
//#include "messages/spa_subscription_reply.h"
//#include "messages/spa_subscription_request.h"


void component_messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;
  comp->handleSpaData(message);
  return;
}


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
  ++publishIter;

  if (publishIter == 201)
  { // Max deliveryRateDivisor is therefore 200
    publishIter = 1;
  }

  auto pid = fork();

  if (pid < 0)
  {
    std::cerr << "Did not fork!..." << std::endl;
  }

  else if (pid == 0) // child process
  {
    /* send data */
    for (;;)
    {
    //  std::this_thread::sleep_for(std::chrono::milliseconds(100));
      std::lock_guard<std::mutex> lock(m_subscribers);
      for (auto i = 0u; i < subscribers.size(); ++i)
      {
        if (subscribers[i].deliveryRateDivisor % publishIter == 0)
        {
          sendSpaData(subscribers[i].subscriberAddress);
        }
      }
    }
  }
  else // parent process
  {
    /* listen for more requests */
    communicator->getLocalCommunicator()->clientListen(
        [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });
  }
}
