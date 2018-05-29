#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME Battery
#define COMP_ADDR la_BATTERY
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
  }

  void sendData(LogicalAddress destination)
  {
    // TODO Write me!?
    sleep(1);
    std::string payload = "Battery data!";
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
