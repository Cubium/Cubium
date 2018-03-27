#include "../demo_addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME CompB
#define COMP_ADDR la_CB
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
    std::string payloadStr(castMessage->st);

    std::cout << "Payload: " << payloadStr << std::endl;
  }

  void sendData(LogicalAddress)
  {
  }

  void init()
  {
    subscribe(la_CA);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
