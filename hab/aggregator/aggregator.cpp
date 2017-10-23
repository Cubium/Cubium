#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#define COMP_NAME Aggregator
#define COMP_ADDR la_AGGREGATOR
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

    std::cout << "Payload: " << payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = "Aggregator data!";
    std::cout << "Sending data: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_TEMPIN);
    sleep(0.5);
    subscribe(la_TEMPEX);
    sleep(0.5);
    subscribe(la_RTC);
    sleep(0.5);
    subscribe(la_UV);
    sleep(0.5);
    subscribe(la_LIGHT);
    sleep(0.5);
    subscribe(la_GYRO);
    sleep(0.5);
    subscribe(la_GPS);
    sleep(0.5);
    subscribe(la_BARO);
    sleep(0.5);
    subscribe(la_FILTER);
    sleep(0.5);
    subscribe(la_BOOM);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
