#include <local_communicator.hpp>

class MockLocalCommunicator : public LocalCommunicator
{
public:
  MockLocalCommunicator(LogicalAddress la) : LocalCommunicator(nullptr, nullptr, la) {}

  MOCK_METHOD1(send, bool(std::shared_ptr<SpaMessage>));
  MOCK_METHOD1(listen, void(PhysicalCommunicator::MessageCallback));
};
