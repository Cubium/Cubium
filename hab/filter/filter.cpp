#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>

#include "medianFilter.hpp"

#define COMP_NAME Filter
#define COMP_ADDR la_FILTER
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
private:
  median_filter<float> filter;

public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
    filter = median_filter<float>();
  }

  void handleSpaData(SpaMessage* message)
  {
    auto castMessage = (SpaData<float>*)message;
    float payload = castMessage->payload;

    //std::cout << "Payload: " << payload << std::endl;

    filter.push(payload);
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);

    float payload = filter.median();

    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_BAROMETER);
    sleep(1);
 }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
