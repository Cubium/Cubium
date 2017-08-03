#ifndef SPA_COMMUNICATOR_HPP
#define SPA_COMMUNICATOR_HPP

#include <functional>
#include <memory>
#include <vector>

#include "local_communicator.hpp"
#include "physical_communicator.hpp"
#include "routing_table.hpp"
#include "spa_message.h"

#define LOCAL_SUBNET_ADDRESS 1

class SpaCommunicator
{
public:
  //! Construct SpaCommunicator with only the address of the owning subnet manager

  //! \param currentAddress - logical address of the subnet manager who owns SpaCommunicator
  SpaCommunicator(LogicalAddress currentAddress);

  //TODO document
  SpaCommunicator(LogicalAddress currentAddress, std::vector<std::shared_ptr<PhysicalCommunicator>> comms);

  //TODO document
  void addCommunicators(std::vector<std::shared_ptr<PhysicalCommunicator>> comms);

  //! Sends a spa message over the network

  //! \param message - Specialization of a Message to be sent over the network.
  //! \return true if message is successfully sent, false otherwise.
  bool send(SpaMessage* message, ssize_t len);

  //TODO document
  virtual void listen(std::function<void(cubiumServerSocket_t*)>);
  virtual void listen(std::function<void(cubiumClientSocket_t*)>);

  std::shared_ptr<LocalCommunicator> getLocalCommunicator();

protected:
  std::vector<std::shared_ptr<PhysicalCommunicator>> communicators;

  //! Method called when something unexpected occurs.
  void handleFailure();

  //! Selects the appropriate physical communicator from list of physical communicators
  //! to send message.

  //! \param address - logical address where message is going to be sent
  //! \param communicators - vector of communicators to be selected from
  std::shared_ptr<PhysicalCommunicator> selectCommunicator(LogicalAddress address, std::vector<std::shared_ptr<PhysicalCommunicator>> const& communicators);

  //TODO document

  LogicalAddress currentAddress;
};

#endif
