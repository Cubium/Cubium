#include <spa_communicator.hpp>

#include "mocks/physical_communicator.hpp"

class SpaCommunicatorTest : public ::testing::Test
{
public:
  virtual void SetUp()
  {
    comms.push_back(
        localCom = std::make_shared<MockPhysicalCommunicator>(LogicalAddress(1, 0)));
    comms.push_back(
        foreignCom = std::make_shared<MockPhysicalCommunicator>(LogicalAddress(2, 0)));

    localMessage = std::make_shared<SpaMessage>(LogicalAddress(1, 1), 1);
    foreignMessage = std::make_shared<SpaMessage>(LogicalAddress(2, 1), 1);
    nonConnectedMessage = std::make_shared<SpaMessage>(LogicalAddress(3, 1), 1);
  }

  std::shared_ptr<MockPhysicalCommunicator> localCom;
  std::shared_ptr<MockPhysicalCommunicator> foreignCom;
  std::vector<SpaCommunicator::Com> comms;
  std::shared_ptr<SpaMessage> localMessage;
  std::shared_ptr<SpaMessage> foreignMessage;
  std::shared_ptr<SpaMessage> nonConnectedMessage;
};

TEST_F(SpaCommunicatorTest, send__to_same_network)
{
  SpaCommunicator spaCom(LogicalAddress(1, 0), comms);
  EXPECT_CALL(*localCom, sendMsg(localMessage))
      .Times(1);

  bool result = spaCom.send(localMessage);
  EXPECT_TRUE(result);
}

TEST_F(SpaCommunicatorTest, send__to_other_network)
{
  SpaCommunicator spaCom(LogicalAddress(1, 0), comms);
  EXPECT_CALL(*foreignCom, sendMsg(foreignMessage))
      .Times(1);

  bool result = spaCom.send(foreignMessage);
  EXPECT_TRUE(result);
}

TEST_F(SpaCommunicatorTest, send__to_non_connected_network)
{
  SpaCommunicator spaCom(LogicalAddress(1, 0), comms);
  EXPECT_CALL(*foreignCom, sendMsg(nonConnectedMessage))
      .Times(0);
  EXPECT_CALL(*localCom, sendMsg(nonConnectedMessage))
      .Times(0);

  bool result = spaCom.send(nonConnectedMessage);
  EXPECT_FALSE(result);
}

TEST_F(SpaCommunicatorTest, listen)
{
  SpaCommunicator spaCom(LogicalAddress(1, 0), comms);

  PhysicalCommunicator::MessageCallback callback = [](uint8_t *buff, uint32_t size) { return; };
  EXPECT_CALL(*localCom, listen(callback))
      .Times(1);

  spaCom.listen(callback);
}

class _SpaCommunicator : public SpaCommunicator
{
public:
  _SpaCommunicator(LogicalAddress currentAddress) : SpaCommunicator(currentAddress) {}
  Com _selectCommunicator(LogicalAddress address, std::vector<Com> const &communicators)
  {
    return selectCommunicator(address, communicators);
  }
};

TEST_F(SpaCommunicatorTest, selectCommunicator)
{
  LogicalAddress l1(1, 0); // Local
  LogicalAddress l2(2, 0); // Foreign
  LogicalAddress l3(3, 0); // Nonconnected

  _SpaCommunicator spaCom(l1);

  auto selected = spaCom._selectCommunicator(l2, comms);
  EXPECT_TRUE(selected->getSubnetAddress() == l2);

  selected = spaCom._selectCommunicator(l1, comms);
  EXPECT_TRUE(selected->getSubnetAddress() == l1);

  selected = spaCom._selectCommunicator(l3, comms);
  EXPECT_TRUE(selected == nullptr);
}
