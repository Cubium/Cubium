#include <iostream>
#include <memory>
#include <messages/local/local_ack.h>
#include <messages/local/local_hello.h>
#include <messages/local/local_spa_message.h>
#include <messages/op_codes.h>
#include <spa_message.h>
#include <subnet_manager.hpp>

std::shared_ptr<SpaCommunicator> SubnetManager::communicator;
