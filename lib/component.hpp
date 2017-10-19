#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "messages/local/local_hello.h"
#include "messages/spa/spa_data.h"
#include "messages/spa/spa_string.h"
#include "spa_communicator.hpp"
#include "spa_message.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>

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
  Component(std::shared_ptr<SpaCommunicator> communicator = nullptr, LogicalAddress address = LogicalAddress(0, 0), LogicalAddress subnetManagerAddress = LogicalAddress(0, 0))
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

  void publish();

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
    communicator->getLocalCommunicator()->sendMsg(message, len);
  }

  void receiveMessage(SpaMessage*);

  void registerSubscriptionRequest(SpaMessage*);

  void subscribe(LogicalAddress producer) { subscribe(producer, 0, 0, 0); }
  void subscribe(
      LogicalAddress producer,
      uint8_t priority,
      uint32_t leasePeriod,
      uint16_t deliveryRateDivisor);

  void sendDataThreaded(LogicalAddress la)
  {
    auto t = std::thread([=](){ sendData(la); });
    t.join();
  }

  virtual void sendData(LogicalAddress) = 0;

  void sendPayload(std::string payload, LogicalAddress destination)
  {
    if (payload.length() > 128)
    {
      std::cout << "Your string is too big stupid. Will be updated eventually.\n";
      return;
    }

    SpaString message(destination, address);

    strcpy(message.st, payload.c_str());
    
    communicator->getLocalCommunicator()->sendMsg((SpaMessage*)&message, sizeof(message));
  }

  template <typename T>
  void sendPayload(T payload, LogicalAddress destination)
  {
    SpaData<T> dataMessage(destination, address, payload);
    communicator->getLocalCommunicator()->sendMsg((SpaMessage*)&dataMessage, sizeof(SpaData<T>));
  }

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

template <typename T>
void component_start(LogicalAddress address)
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, address)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(address, comms);

  auto comp = std::make_shared<T>(spaCom);
  comp->preInit();
  comp->init();
  comp->listen();
}

#endif
