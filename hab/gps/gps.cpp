#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME Gps
#define COMP_ADDR la_GPS
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
    std::string payload = "GPS data!";
    std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
