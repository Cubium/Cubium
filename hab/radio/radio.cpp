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
    std::cout << "Sleeping 5 seconds...\n";
    sleep(5);
    std::ifstream file("commands.txt");
    std::string payload;
    std::getline(file, payload);
    if (payload != "")
    {
      std::cout << "Sending payload: " << payload << std::endl;
      sendPayload(payload, destination);
    }
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
