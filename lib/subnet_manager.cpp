#include <iostream>
#include <spa_message.h>
#include <subnet_manager.hpp>
#include <messages/op_codes.h>
#include <messages/local/local_ack.h>
#include <memory>
#include <messages/local/local_spa_message.h>
#include <messages/local/local_hello.h>

std::shared_ptr<SpaCommunicator> SubnetManager::communicator;

