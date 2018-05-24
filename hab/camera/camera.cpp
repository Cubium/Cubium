#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME Camera
#define COMP_ADDR la_CAMERA
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    auto castMessage = (SpaString*)message;
    std::string payload(castMessage->st);

    //std::cout << "Payload: " << payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    // TODO Write me!
    std::string payload = "yo foo I took a photo";
    //std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_BOOM);
    sleep(0.5);
    subscribe(la_RADIO);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
