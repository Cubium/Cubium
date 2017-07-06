#ifndef MOCK_SPA_COMMUNICATOR
#define MOCK_SPA_COMMUNICATOR
#include <spa_communicator.hpp>

class MockSpaCommunicator : public SpaCommunicator
{
public:
  MockSpaCommunicator() : SpaCommunicator(LogicalAddress(1, 0)) {}

  MOCK_METHOD1(listen, void(PhysicalCommunicator::MessageCallback));
};
#endif
