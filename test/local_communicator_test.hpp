#include "mocks/platform_abstraction/socket/server_socket.hpp"
#include <local_communicator.hpp>
#include <routing_table.hpp>
using ::testing::DefaultValue;

class LocalCommunicatorTest : public ::testing::Test
{
public:
  virtual void SetUp()
  {
    opcode = 0;
    port = 9;

    addr = std::make_shared<LogicalAddress>(1, 1);
    fullTable = std::make_shared<RoutingTable>();
    emptyTable = std::make_shared<RoutingTable>();
    fullTable->insert(*addr, port);
    message = std::make_shared<SpaMessage>(*addr, opcode);
    socket = std::make_shared<MockServerSocket>();
  }

  std::shared_ptr<MockServerSocket> socket;
  std::shared_ptr<RoutingTable> fullTable;
  std::shared_ptr<RoutingTable> emptyTable;
  std::shared_ptr<LogicalAddress> addr;
  std::shared_ptr<SpaMessage> message;
  uint16_t port;
  uint8_t opcode;
};

TEST_F(LocalCommunicatorTest, send__address_exists_in_table)
{
  LocalCommunicator lc(
      socket,
      fullTable,
      *addr);

  uint8_t *expectedBuff = nullptr;
  uint32_t expectedBuffLen = message->marshal(expectedBuff);

  EXPECT_CALL(*socket, send(SERVER, port, ::testing::_, expectedBuffLen))
      .Times(1);

  bool result = lc.sendMsg(message);
  EXPECT_TRUE(result);
}

TEST_F(LocalCommunicatorTest, send__not_in_table)
{
  LocalCommunicator lc(
      socket,
      emptyTable,
      *addr);

  uint8_t *expectedBuff = nullptr;
  uint32_t expectedBuffLen = message->marshal(expectedBuff);

  EXPECT_CALL(*socket, send(SERVER, -1, expectedBuff, expectedBuffLen))
      .Times(0);

  bool result = lc.sendMsg(message);
  EXPECT_FALSE(result);
}
