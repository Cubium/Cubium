#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

#define COMP_NAME Boom
#define COMP_ADDR la_BOOM
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    sleep(1);
    auto castMessage = (SpaString*)message;
    std::string payload(castMessage->st);

    std::cout << "Payload: " << payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
//    auto t = std::thread([=](){
    sleep(1);
    std::string payload = "Boom message!";
    std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
 //   });
 //   t.join();
  }

  void init()
  {
    subscribe(la_FILTER);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
