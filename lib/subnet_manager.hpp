#ifndef SUBNET_MANAGER
#define SUBNET_MANAGER

#include <memory>
#include <routing_table.hpp>
#include <spa_communicator.hpp>
#include <logical_address.h>
#include <functional>
#include <messages/local/local_spa_message.h>

class SubnetManager
{
public:
  SubnetManager(std::shared_ptr<SpaCommunicator> com, LogicalAddress log , uint16_t port)
  {
	communicator = com;
	routingTable = std::make_shared<RoutingTable>(log, port);
  }

  // Specialization methods
  //
  // The following methods should be implemented by any class that extends
  // SubnetManager

  // virtual bool healthCheck(Component component);

  //! Method called whenever a method is received. This should serve as an
  //! entry point for logic that handles recieving mesages

  //! \param message - message that has been received
  // virtual void recieveMessage(SpaMessage message);

  // Subnet lifecycle methods
  //
  // These blocking methods start long running tasks, and should be called upon creation
  // of a subnet manager

  //! Continuously ping components checking to make sure they are still responsive.
  //! Should report component failture to logging service. This function should
  //! not return while the subnet manager is running.
  // void monitorHealth();

  //! Continuously listen for messages. Will call receiveMessage with each received
  //! message. A call to this method should not return while the subnet manager is running.
  void listenMessages(std::function<void(cubiumServerSocket_t*)> func)
  {
    if (communicator)
    {
      communicator->listen(func);
    }
  }

  // Subnet Manager Utilities
  //
  // These methods will be used internally by the subnet manager, and should be applicable to
  // all instances of subnet manager

  //! Run a long running task on a new thread

  //! \param task - lambda function to run on a seperate thread
  // template<typename Func>
  // void runTask(Func task);

protected:
  static std::shared_ptr<SpaCommunicator> communicator;
	std::shared_ptr<RoutingTable> routingTable;
  
  // TODO add component list to store data about component health
};
#endif
