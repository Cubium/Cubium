#ifndef SPA_COMMUNICATOR_HPP
#define SPA_COMMUNICATOR_HPP

#include <memory>
#include <vector>
#include <functional>


#include "physical_communicator.hpp"
#include "routing_table.hpp"
#include "spa_message.h"

#define LOCAL_SUBNET_ADDRESS 1

class SpaCommunicator
{
public:
  typedef std::shared_ptr<PhysicalCommunicator> Com;

  //! Construct SpaCommunicator with only the address of the owning subnet manager

  //! \param currentAddress - logical address of the subnet manager who owns SpaCommunicator
  SpaCommunicator(LogicalAddress currentAddress);

  //TODO document
  SpaCommunicator(LogicalAddress currentAddress, std::vector<Com> comms);

  //TODO document
  void addCommunicators(std::vector<Com> comms);

  //! Sends a spa message over the network

  //! \param message - Specialization of a Message to be sent over the network.
  //! \return true if message is successfully sent, false otherwise.
  bool send(SpaMessage* message);

  //TODO document
  virtual void listen(std::function<void(void *, uint32_t)>);

  Com getLocalCommunicator();

protected:
  //! Method called when something unexpected occurs.
  void handleFailure();

  //! Selects the appropriate physical communicator from list of physical communicators
  //! to send message.

  //! \param address - logical address where message is going to be sent
  //! \param communicators - vector of communicators to be selected from
  Com selectCommunicator(LogicalAddress address, std::vector<Com> const &communicators);

  //TODO document

  LogicalAddress currentAddress;
  std::vector<Com> communicators;
};

#endif
