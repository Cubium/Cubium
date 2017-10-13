#include "../addresses.hpp"
#include <component.hpp>
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
    auto castMessage = (SpaData<std::string>*)message;
    std::cout << "Payload: " << castMessage->payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = "Command from radio!";
    sendPayload(payload, destination);
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
