//Handle health checks
//Handle componenet registration

#ifndef LOCAL_SUBNET_MANAGER
#define LOCAL_SUBNET_MANAGER

#include "component_list.hpp"
#include "subnet_manager.hpp"
#include <cstdint>

class LocalSubnetManager : public SubnetManager
{
public:
  LocalSubnetManager(std::shared_ptr<SpaCommunicator> c, LogicalAddress log, uint16_t port) : SubnetManager(c, log, port) {}
  void receiveMessage(SpaMessage* message);
  ComponentList components;
};
#endif
