#ifndef LOCAL_SUBNET_MANAGER
#define LOCAL_SUBNET_MANAGER

#include "component_list.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

class LocalSubnetManager;

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

class LocalSubnetManager : public std::enable_shared_from_this<LocalSubnetManager>
{
public:
  LocalSubnetManager(std::shared_ptr<LocalCommunicator> c, std::shared_ptr<RoutingTable<cubiumServerSocket_t>> rt)
  {
    communicator = c;
    routingTable = rt;
  }

  void listenMessages()
  {
    communicator->listen(
      [&](cubiumServerSocket_t* sock)
      { 
        LSM_messageCallback(LocalSubnetManager::shared_from_this(), sock); 
      }
    );
  }

  friend void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);
  friend void LSM_sendMessage(std::shared_ptr<LocalSubnetManager> const lsm, std::size_t const size, SpaMessage* msg);

  static std::shared_ptr<LocalCommunicator> communicator;

private:
  ComponentList components;
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
