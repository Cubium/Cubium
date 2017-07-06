#include "mocks/spa_communicator.hpp"
#include <component.hpp>
#include <local_communicator.hpp>
#include <local_subnet_manager.hpp>
#include <logical_address.hpp>

class LocalSubnetManagerTest : public ::testing::Test
{
public:
  virtual void SetUp()
  {
    opcode = op_LOCAL_HELLO;
    spaCom = std::make_shared<MockSpaCommunicator>();
  }
  uint8_t opcode;
  std::shared_ptr<MockSpaCommunicator> spaCom;
};

TEST_F(LocalSubnetManagerTest, add_component)
{
  LogicalAddress source(1, 1);
  LogicalAddress dest(1, 0);
  auto msg = std::make_shared<SpaMessage>(0, 0, 0, dest, source, 0, opcode);
  LocalSubnetManager lsm(spaCom);

  // Sends LocalHello from a component at (1,1) to the LSM
  lsm.receiveMessage(msg);

  // Passes if the LSM adds the component's address to the ComponentList
  EXPECT_TRUE(lsm.components.getAddress(0) == source);
}
