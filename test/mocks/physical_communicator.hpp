#include <physical_communicator.hpp>

class MockPhysicalCommunicator : public PhysicalCommunicator
{
public:
  MockPhysicalCommunicator(LogicalAddress la) : PhysicalCommunicator(la) {}

  MOCK_METHOD1(sendMsg, bool(std::shared_ptr<SpaMessage>));
  MOCK_METHOD1(listen, void(PhysicalCommunicator::MessageCallback));
};
