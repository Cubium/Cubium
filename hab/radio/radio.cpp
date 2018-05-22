#include "../addresses.hpp"
#include <component.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h>

#define COMP_NAME Radio
#define COMP_ADDR la_RADIO
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

    std::cout << "Got payload: " << payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    std::string payload;
    std::getline(std::cin, payload);

    if (payload == "deploy")
    {
      std::cout << "Sending deployment in 5 seconds...\n";
      sleep(5);
      std::cout << "Sending command: " << payload << std::endl;
      sendPayload(payload, destination);
    }
  }

  void init()
  {
    subscribe(la_AGGREGATOR);
    subscribe(la_CAMERA);
  }
private:
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
