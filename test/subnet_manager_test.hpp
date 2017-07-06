#include "mocks/spa_communicator.hpp"
#include <subnet_manager.hpp>

class SubnetManagerTest : public ::testing::Test
{
public:
  virtual void SetUp() {}
};

// TEST_F(SubnetManagerTest, constructor){
// std::shared_ptr<MockSpaCommunicator> com = std::make_shared<MockSpaCommunicator>();
// EXPECT_CALL(*com, listen(testing::_)).Times(1);
// SubnetManager manager(com);
// }
