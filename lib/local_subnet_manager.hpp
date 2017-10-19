//Handle health checks
//Handle componenet registration

#ifndef LOCAL_SUBNET_MANAGER
#define LOCAL_SUBNET_MANAGER

#include "component_list.hpp"
#include "subnet_manager.hpp"
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

class LocalSubnetManager;

void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

class LocalSubnetManager : public SubnetManager, public std::enable_shared_from_this<LocalSubnetManager>
{
public:
  LocalSubnetManager(std::shared_ptr<SpaCommunicator> c, std::shared_ptr<RoutingTable<cubiumServerSocket_t>> rt)
    : SubnetManager(c)
  {
    routingTable = rt;
  }

  void listenMessages()
  {
    if (communicator)
    {
      auto callback = std::bind(LSM_messageCallback, LocalSubnetManager::shared_from_this(), std::placeholders::_1);
      communicator->getLocalCommunicator()->listen(callback);
    }
  }
  void receiveMessage(SpaMessage* message) {} // Note: Deprecated?
  friend void LSM_messageCallback(std::shared_ptr<LocalSubnetManager> lsm, cubiumServerSocket_t* sock);

private:
  ComponentList components;
  std::shared_ptr<RoutingTable<cubiumServerSocket_t>> routingTable;
};

#endif
