#ifndef LOCAL_SUBNET_MANAGER
#define LOCAL_SUBNET_MANAGER

#include "component_list.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "messages/local/all_registered.h"
#include "messages/local/all_subscribed.h"

/* Literally for the memes. */
#define ifnt else

/* Forward declaration of the LSM class for the callback function */
class LocalSubnetManager;

/* 
 * Forward declaration for the callback function.
 */
int LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

/*
 * The LocalSubnetManager routes all messages between components and enforces the 
 * separate phases of Cubium's operation.
 * Inherits from std::enable_shared_from_this to allow smart pointers to the LSM
 * to be passed to its message callback function.
 */
class LocalSubnetManager : public std::enable_shared_from_this<LocalSubnetManager>
{
public:
  /*
   * The one-and-only constructor for the LSM.
   *
   * @param c  The communicator to use
   * @param rt The routing table to use
   * @param n  The number of components the LSM is to serve
   * @param s  The total number of subscriptions the LSM must route
   * @param la The logical address of the LSM
   */
  LocalSubnetManager(std::shared_ptr<LocalCommunicator> c, std::shared_ptr<RoutingTable<cubiumServerSocket_t>> rt, int n, int s, LogicalAddress la)
  : address(la)
  {
    communicator = c;
    routingTable = rt;
    numComponents = n;
    numSubs = s;
  }


  /*
   * Listen for messages. That's about it.
   */
  void listenMessages()
  {
    communicator->listen(
        [&](cubiumServerSocket_t* sock) {
          return LSM_messageCallback(LocalSubnetManager::shared_from_this(), sock);
        });
  }

  /*
   * Listen for messages, but throw a runtime error if the listen function
   * runs for longer than 8 seconds.
   *
   * Useful for enforcing Cubium phases.
   */
  void timeOut_listenMessages()
  {
    std::mutex m;
    std::condition_variable cv;

    std::thread t([&m, &cv, this]()
    {
      listenMessages();
      cv.notify_one();
    });

    t.detach();

    {
      std::unique_lock<std::mutex> l(m);
      if (cv.wait_for(l, std::chrono::seconds(8)) == std::cv_status::timeout)
      {
        throw std::runtime_error("Timeout");
      }
    }
  }
  
  /*
   * A simple enum to keep track of phases
   */
  enum Phase
  {
    INIT,
    SUBSCRIBE
  };

  /*
   * Start up a phase, and end the phase to proceed to the next one
   * if the current phase times out.
   *
   * @param Phase The phase to begin.
   */
  void doPhase(Phase phase)
  {
    bool timedOut = false;
    try 
    {
      timeOut_listenMessages();
    }
    catch (std::runtime_error& e)
    {
      std::cout << e.what();
      timedOut = true;
    }

    if (timedOut)
    {
      if (phase == Phase::INIT)
      {
        notifyComponents(op_ALL_REGISTERED);
      }
      ifnt if (phase == Phase::SUBSCRIBE)
      {
        notifyComponents(op_ALL_SUBSCRIBED);
      }
    }

    return;
  }

  /*
   * Begin LSM operation. Starts by enforcing the init phase,
   * then the subscribe phase, and then simply listens for messages to route.
   */
  void start()
  {
    doPhase(Phase::INIT);
    doPhase(Phase::SUBSCRIBE);
    listenMessages();
  }

  /*
   * Send a network-wide notification to all components that a phase has ended.
   * Only supports ALL_REGISTERED and ALL_SUBSCRIBED messages.
   * @param op The opcode of the message to send.
   */
  void notifyComponents(uint8_t op)
  {
    for(auto i = 0u; i < components.getSize(); ++i)
    {
      LogicalAddress dest = components.getAddress(i);
      //std::cout << "Sending message to :" << dest << "\n";
      if (op == op_ALL_REGISTERED)
      {
        auto msg = AllRegistered(dest, LogicalAddress(1,0));
        LSM_sendMessage(shared_from_this(), sizeof(msg), (SpaMessage*)&msg);
      }
      ifnt if (op == op_ALL_SUBSCRIBED)
      {
        auto msg = AllSubscribed(dest, LogicalAddress(1,0));
        LSM_sendMessage(shared_from_this(), sizeof(msg), (SpaMessage*)&msg);
      }
      ifnt
      {
        perror("Wat u doin foo");
      }

    }
  }

  /*
   * Check to see if every component has registered with the LSM.
   */
  bool allRegistered() { 
   // std::cout << "Components: " << components.getSize() << std::endl; 
    return components.getSize() == numComponents; }

  /*
   * Check to see if every component has successfully sent a SubscriptionReply
   * and received a SubscriptionRequest for each of their subscriptions.
   */
  bool allSubscribed()
  {
    return replyCount == numSubs;
  }

  /* 
   * The callback function that is called when the LSM receives a message.
   * @param lsm  A pointer to the LSM
   * @param sock The socket that the message came through
   */
  friend int LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

  /*
   * Send a message to a component.
   * @param lsm  A pointer to the LSM
   * @param size The size of the message
   * @param msg  The message
   */
  friend void LSM_sendMessage(std::shared_ptr<LocalSubnetManager> const lsm, std::size_t const size, SpaMessage* msg);

  /* communicator   The communicator that the LSM uses to send messages through sockets */
  static std::shared_ptr<LocalCommunicator> communicator;


  /*
   * Deprecated.
   * TODO remove
   */
  bool subscriptionsAllowed() 
  { 
    return subsAllowed;
  }

  /*
   * Deprecated.
   * TODO remove
   */
  void disallowSubs() 
  { 
    subsAllowed = false; 
  }

  /*
   * Deprecated.
   * TODO remove
   */
  void allowSubs()
  {
    subsAllowed = true;
  }

  /*
   * Increment the number of SubscriptionReplies that the LSM has seen
   */
  void incrementSubs() { ++replyCount; }

  /* address   The LSM's logical address */
  LogicalAddress address;

private:
  /* subsAllowed   Deprecated TODO remove */
  bool subsAllowed = true;
  /* replyCount  The number of SubscriptionReplies that the LSM has seen*/
  int replyCount = 0;

  /* subber   Deprecated TODO remove */
  std::shared_ptr<LogicalAddress> subber = nullptr;

  /* numComponents   The number of components the LSM serves */
  int numComponents;

  /* numSubs   The number of subscriptions the LSM must route */
  int numSubs;

  /* components  The list of components the LSM serves */
  ComponentList components;

  /* routingTable    The all-holy routing table */
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
