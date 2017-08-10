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

  void handleSpaData(SpaData* message)
  {
    std::cout << "Payload: " << message->payload << std::endl;
  }

  float packageData()
  {
    sleep(1);
    auto payload = rand() % 100;

    std::cout << "Sending SpaData: " << payload << std::endl;
    
    return payload;
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
