#include "../demo_addresses.hpp"
#include "messages/op_codes.h"
#include "random.hpp"
#include <chrono> // std::chrono::seconds
#include <component.hpp>
#include <iostream>
#include <local_communicator.hpp>
#include <local_component_routing_table.hpp>
#include <messages/local/local_hello.h>
#include <messages/spa/spa_data.h>
#include <messages/spa/subscription_reply.h>
#include <messages/spa/subscription_request.h>
#include <socket/clientSocket.hpp>
#include <thread> // std::this_thread::sleep_for
#include <unistd.h>

class ComponentB;

void messageCallback(std::shared_ptr<ComponentB> comp, cubiumClientSocket_t* sock);

int readData();

class ComponentB : public Component, public std::enable_shared_from_this<ComponentB>
{
public:
  ComponentB(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com) {}

  virtual void handleSpaData(SpaMessage*) {}
  virtual void sendSpaData(LogicalAddress) {}

  virtual void appInit()
  {
    std::cout << "Component B initializing!" << '\n';

    LocalHello hello(0, 0, la_LSM, la_CB, 0, 0, 0, 0);

    communicator->getLocalCommunicator()->clientConnect((SpaMessage*)&hello, sizeof(hello), [=](cubiumClientSocket_t* s) { messageCallback(ComponentB::shared_from_this(), s); });

    auto pid = fork();
    if (pid < 0)
      std::cerr << "Did not fork!..." << std::endl;
    else if (pid == 0) // child process
    {
      /* send data */
      for (;;)
      {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << subscribers.size() << "              SEND DATA " << std::endl;
        auto data = readData();
        std::lock_guard<std::mutex> lock(m_subscribers);
        for (int i = 0; i < subscribers.size(); ++i)
        {
          std::cout << "Sending Data" << '\n';
          SpaData<int> data(0, 0, la_CB, subscribers[i].subscriberAddress, 0, 0, 0, 0, 0, 5);
          communicator->getLocalCommunicator()->sendMsg((SpaMessage*)&data, sizeof(data));
        }
      }
    }
    else // parent process
    {
      /* listen for more requests */
      communicator->getLocalCommunicator()->clientListen(
          [=](cubiumClientSocket_t* s) { messageCallback(ComponentB::shared_from_this(), s); });
    }
  }
};

void messageCallback(std::shared_ptr<ComponentB> comp, cubiumClientSocket_t* sock)
{
  SpaMessage* message = (SpaMessage*)sock->buf;
  auto op = message->spaHeader.opcode;
  std::cout << "Received SpaMessage with opcode: " << (int)op << '\n';

  if (op == op_SPA_SUBSCRIPTION_REQUEST)
  {
    SubscriptionReply reply(message->spaHeader.source, la_CB);
    comp->communicator->getLocalCommunicator()->clientSend((SpaMessage*)&reply, sizeof(SubscriptionReply));
    if (comp->addSubscriber(message->spaHeader.source, 0))
      std::cout << "Added " << message->spaHeader.source << " as a subscriber" << std::endl;
  }

  return;
}

int readData()
{
  static int data = 0;
  int random = rand(1, 100);
  if (random < 25)
  {
    ++data;
    return random;
  }
  return ++data;
}

int main()
{
  cubiumClientSocket_t sock = clientSocket_openSocket(3500);
  auto routingTable = std::make_shared<RoutingTable<cubiumServerSocket_t>>();

  std::vector<std::shared_ptr<PhysicalCommunicator>> comms = {
      std::make_shared<LocalCommunicator>(&sock, routingTable, la_CB)};
  std::shared_ptr<SpaCommunicator> spaCom = std::make_shared<SpaCommunicator>(la_CB, comms);

  auto comp = std::make_shared<ComponentB>(spaCom);
  comp->appInit();

  return 0;
}
