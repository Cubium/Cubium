#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "local_communicator.hpp"
#include "messages/local/local_hello.h"
#include "messages/spa/spa_data.h"
#include "messages/spa/spa_string.h"
#include "spa_message.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

/*
 * The subscriber struct is simply a convenient
 * pairing of a logical address and delivery rate
 * divisor for messages sent to that subscriber.
 *
 * This feature is currently not fully implemented.
 * We do not currently use the divisor rate feature
 *
 * @param subscriberAddress   The address of the subscriber
 * @param deliveryRateDivisor The rate at which messages are sent to that subscriber
 */
struct Subscriber
{
  Subscriber(LogicalAddress la, uint16_t d)
    : subscriberAddress(la), deliveryRateDivisor(d) {}
  LogicalAddress subscriberAddress;
  uint16_t deliveryRateDivisor;
};

// forward declaration of Component class
class Component;

/*
 * This function is called whenever a component receives a message
 * @param comp The component that is receiving the message
 * @param sock Information struct of the socket from which the message came
 */
void component_messageCallback(std::shared_ptr<Component> comp, cubiumClientSocket_t* sock);

/*
 * The main component class from which all Cubium components inherit.
 */
class Component : public std::enable_shared_from_this<Component>
{
public:
  /*
   * The constructor initializes and reserves memory for the vector of subscribers.
   * @param communicator         The communicator for the component to use
   * @param address              The logical address for the component to use
   * @param subnetManagerAddress The logical address of the subnet manager
   */
  Component(std::shared_ptr<LocalCommunicator> communicator = nullptr, LogicalAddress address = LogicalAddress(0, 0), LogicalAddress subnetManagerAddress = LogicalAddress(0, 0))
    : communicator(communicator),
      address(address),
      subnetManagerAddress(subnetManagerAddress)
  {
    subscribers.reserve(8); // Default to 8 subscribers
  }

  /*
   * Note: Do not delete the virtual destructor.
   * Everything will break because that would remove the default destructors
   * from the inherited classes.
   */
  virtual ~Component() {}

  /*
   * Spins up a thread to initiate the data-publishing loop wherein SpaData
   * is gathered from the component interface and sent to each subscriber,
   * and then begins listening for messages.
   */
  void publish();

  /*
   * Virtual method to be defined in the child-class component interface
   */
  virtual void handleSpaData(SpaMessage*) = 0;

  /*
   * The first function that is run after component construction.
   * Sends a LocalHello to the subnet manager in order to be
   * registered in the routing table.
   */
  virtual void registerWithSubnetManager()
  {
    LocalHello hello(0, 0, subnetManagerAddress, address, 0, 0, 0, 0);

    communicator->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); });
  }

  /*
   * Listen for messages coming through the socket.
   * The callback function is passed to handle received data
   */
  virtual void listen()
  {
    communicator->clientListen(
        [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); }, 0);
  }

  /*
   * This function operates just the same as Component::listen(), but it
   * will terminate when it receives a message with the provided opcode.
   * It is used to enforce the separate initialization phases.
   *
   * @param exitOp The termination opcode
   */
  virtual void waitFor(uint8_t exitOp)
  {
    communicator->clientListen(
        [=](cubiumClientSocket_t* s) { component_messageCallback(shared_from_this(), s); }, exitOp);
  }



  /*
   * This function initializes a component driver.
   * It is a function defined by the user to initialize the component's driver
   * This is one of the functions to be implemented by a child class.
   */
  virtual void init() = 0;

  /*
   * Pass a message to the communicator.
   * The message knows its destination.
   * @param message The message to be sent
   * @param len     The size of the message
   */
  void sendMsg(SpaMessage* message, ssize_t len)
  {
    if (message == nullptr || communicator == nullptr)
    {
      return;
    }
    communicator->sendMsg(message, len);
  }

  /*
   * This function is called in component_messageCallback when a message
   * is received. It determines what kind of message it is and calls
   * the appropriate handler function.
   *
   * @param message The message that has been received
   */
  void receiveMessage(SpaMessage* message);

  /*
   * Called when a subscription request is received.
   * Sends a SubscriptionReply to the message source and adds the sender
   * to the list of subscribers.
   *
   * @param message The SubscriptionRequest received.
   */
  void registerSubscriptionRequest(SpaMessage* message);

  /*
   * Subscribe to another component.
   *
   * @param producer The address of the component to subscribe to
   * @param priority The priority by which to receive SpaData (not implemented)
   * @param leasePeriod How long to stay subscribed (not implemented)
   * @param deliveryRateDivisor How frequently to receive messages (not implemented)
   */
  void subscribe(LogicalAddress producer) { subscribe(producer, 0, 0, 0); }
  void subscribe(
      LogicalAddress producer,
      uint8_t priority,
      uint32_t leasePeriod,
      uint16_t deliveryRateDivisor);

  /*
   * Spin up a thread to send SpaData through
   *
   * @param la The address of the component to send data to.
   */
  void sendDataThreaded(LogicalAddress la)
  {
    // TODO do we even need this? Maybe don't use another thread.
    auto t = std::thread([=]() { sendData(la); });
    t.join();
  }

  /*
   * A virtual function to be defined in the child-class component interface.
   *
   * Package data and send it to another component.
   * It is do be defined by the user.
   */
  virtual void sendData(LogicalAddress) = 0;

  /*
   * Send a string payload to a specified component.
   * 256 is currently an arbitrarily-determined maximum length,
   * as an entirely separate messaging protocol would need to be
   * developed to allow for variable-length messages.
   * TODO future improvement: Allow the user to define this
   * constant in a header file.
   *
   * @param payload The SpaData payload
   * @param destination The component to send the payload to
   */
  void sendPayload(std::string payload, LogicalAddress destination)
  {
    if (payload.length() > 256) // 256 is the max payload length
    {
      std::cout << "Your string is too big:" << payload << std::endl;
      std::cout << "Current string size limit: " << 256 << std::endl;
      return;
    }

    SpaString message(destination, address);

    strcpy(message.st, payload.c_str());

    communicator->sendMsg((SpaMessage*)&message, sizeof(message));
  }

  /*
   * Send any kind of payload to a specified subscriber.
   *
   * @param payload The SpaData payload
   * @param destination The component to send the payload to
   *
   * @warning can only send data types < 128 bytes
   */
  template <typename T>
  void sendPayload(T payload, LogicalAddress destination)
  {
    SpaData<T> dataMessage(destination, address, sizeof(SpaData<T>), payload);
    communicator->sendMsg((SpaMessage*)&dataMessage, sizeof(SpaData<T>));
  }

  /*
   * Add a subscriber to the subscriber list.
   *
   * @param la The subscriber's logical address
   * @param d  The deliveryRateDivisor (unused)
   */
  bool addSubscriber(LogicalAddress la, uint16_t d);

  /*
   * This simple function is called when a component receives
   * a SubscriptionReply, and simply prints an error if the 
   * subscription failed.
   *
   * @param message The SubscriptionReply
   */
  void checkForSubscriptionFailure(SpaMessage* message);


  /*
   * A simple sleep function that enables telling components to sleep
   * from separate scopes, contexts, and threads.
   *
   * @param n The sleep time in seconds
   */
  void compSleep(int n);

  /*
   * communicator  The component's communicator, used to communicate with the subnet manager.
   */
  std::shared_ptr<LocalCommunicator> const communicator;
  /*
   * subscribers   The list of the component's subscribers
   */
  std::vector<Subscriber> subscribers;
  /*
   * m_subscribers A mutex used for accessing the subscriber list since multiple threads access it at once.
   */
  std::mutex m_subscribers;

protected:
  /*
   * address The component's logical address
   */
  LogicalAddress const address;
  /*
   * @param subnetManagerAddress The address of the subnet manager
   */
  LogicalAddress const subnetManagerAddress;
  /*
   * To be implemented probably never.
  uint8_t publishIter;
  uint16_t dialogId;
   */
};

/*
 * A generic function to bootstrap the component.
 * Constructs everything that the component needs,
 * calls appropriate initialization, and begins listening
 * for messages.
 *
 * The phases of component operation are evident from
 * the functions called on the comp object, and are as follows:
 *
 * Phase 0: Register with the subnet manager
 * Phase 1: Initialize drivers and make subscriptions
 * Phase 2: Publish data and listen for messages
 *
 * By using the waitFor() function, no new phase can be entered until
 * ALL other components have completed the work in the previous phase.
 * This avoids messaging conflicts that can cause components to 
 * get stuck initializing. 
 *
 * @param address The logical address to use for the component.
 */
template <typename T>
void component_start(LogicalAddress const& address)
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto const routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();
  auto const communicator = std::make_shared<LocalCommunicator>(&sock, routingTable, address);
  auto const comp = std::make_shared<T>(communicator);

  comp->registerWithSubnetManager();
//  std::cout << "Registered, waiting...\n";
  comp->waitFor(op_ALL_REGISTERED);
//  std::cout << "Everyone is ready!...\n";
  comp->compSleep(2);
//  std::cout << "Initializing with init!\n";
  comp->init();
//  std::cout << "Initialized, waiting for okay...\n";
  comp->waitFor(op_ALL_SUBSCRIBED);
//  std::cout << "Everyone is subscribed! Publishing and listening...\n";
  comp->publish();
}

#endif
