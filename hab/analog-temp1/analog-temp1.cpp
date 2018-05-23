#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME AnalogTemp1
#define COMP_ADDR la_ANALOG_TEMP1
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    float payload = 69.0;
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
