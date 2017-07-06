#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
/*
#include "routing_table_test.hpp"
#include "spa_communicator_test.hpp"
// #include "network_communicator_test.hpp"
#include "component_test.hpp"
#include "local_communicator_test.hpp"
#include "local_subnet_manager_test.hpp"
#include "messages/spa_data_test.hpp"
#include "messages/spa_subscription_reply_test.hpp"
#include "messages/spa_subscription_request_test.hpp"
#include "spa_message_test.hpp"
#include "subnet_manager_test.hpp"
#include "aeroboom_test.hpp"
*/
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
