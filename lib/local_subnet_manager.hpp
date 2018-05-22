#ifndef LOCAL_SUBNET_MANAGER
#define LOCAL_SUBNET_MANAGER

#include "component_list.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <thread>
#include "messages/local/all_registered.h"
#include "messages/local/all_subscribed.h"

class LocalSubnetManager;

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

class LocalSubnetManager : public std::enable_shared_from_this<LocalSubnetManager>
{
public:
  LocalSubnetManager(std::shared_ptr<LocalCommunicator> c, std::shared_ptr<RoutingTable<cubiumServerSocket_t>> rt, int n, int s)
  {
    communicator = c;
    routingTable = rt;
    numComponents = n;
    numSubs = s;
  }

  void listenMessages()
  {
    communicator->listen(
        [&](cubiumServerSocket_t* sock) {
          LSM_messageCallback(LocalSubnetManager::shared_from_this(), sock);
        });
  }

  void notifyComponents(uint8_t op)
  {
    for(auto i = 0u; i < components.getSize(); ++i)
    {
      LogicalAddress dest = components.getAddress(i);
      std::cout << "Sending message to :" << dest << "\n";
      if (op == op_ALL_REGISTERED)
      {
        auto msg = AllRegistered(dest, LogicalAddress(1,0));
        LSM_sendMessage(shared_from_this(), sizeof(msg), (SpaMessage*)&msg);
      }
      else if (op == op_ALL_SUBSCRIBED)
      {
        auto msg = AllSubscribed(dest, LogicalAddress(1,0));
        LSM_sendMessage(shared_from_this(), sizeof(msg), (SpaMessage*)&msg);
      }
      else
      {
        perror("Wat u doin foo");
      }

    }
  }

  bool allRegistered() { 
    std::cout << "Components: " << components.getSize() << std::endl; 
    return components.getSize() == numComponents; }

  bool allSubscribed()
  {
    return replyCount == numSubs;
  }

  friend void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);
  friend void LSM_sendMessage(std::shared_ptr<LocalSubnetManager> const lsm, std::size_t const size, SpaMessage* msg);

  static std::shared_ptr<LocalCommunicator> communicator;

  bool subscriptionsAllowed() 
  { 
    return subsAllowed;
  }

  void disallowSubs() 
  { 
    subsAllowed = false; 
  }

  void allowSubs()
  {
    subsAllowed = true;
  }

  void incrementSubs() { ++replyCount; }

private:
  bool subsAllowed = true;
  int replyCount = 0;
  std::shared_ptr<LogicalAddress> subber = nullptr;
  int numComponents;
  int numSubs;
  ComponentList components;
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
