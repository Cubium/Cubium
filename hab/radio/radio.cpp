#include "../addresses.hpp"
#include <component.hpp>
#include <thread>
#include <iostream>
#include <unistd.h>

#define COMP_NAME Radio
#define COMP_ADDR la_RADIO
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    auto castMessage = (SpaString*)message;
    std::string payload(castMessage->st);

    std::cout << "Payload: " << payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    auto t = std::thread([=](){
    sleep(1);
    std::string payload = "Command from radio!";
    std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
    });
    t.join();
  }

  void init()
  {
    subscribe(la_AGGREGATOR);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
