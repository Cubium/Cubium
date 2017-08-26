#include <component.hpp>
#include <iostream>
#include <unistd.h>
#include "../demo_addresses.hpp"

#define COMP_NAME CompA
#define COMP_ADDR la_CA
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR) 
  { }

  void handleSpaData(SpaMessage* message)
  {

    std::cout << "Payload: Got it \n";//" << message->payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = "You can send over anything you want! Anything at all!";

    std::cout << "Sending SpaData: " << payload << std::endl;

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
