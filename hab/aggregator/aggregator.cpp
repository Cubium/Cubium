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
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    sleep(1);
    auto castMessage = (SpaData<float>*)message;
    std::cout << "Payload: " << castMessage->payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    float payload = 0;
    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_TEMPIN);
    sleep(0.5);
    subscribe(la_TEMPEX);
    sleep(0.5);
//    subscribe(la_RTC);
 //   subscribe(la_UV);
//    subscribe(la_LIGHT);
//    subscribe(la_GYRO);
    subscribe(la_GPS);
//    subscribe(la_BARO);
//    subscribe(la_FILTER);
//    subscribe(la_BOOM);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
