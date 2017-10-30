#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdio.h>

#define COMP_NAME Boom
#define COMP_ADDR la_BOOM
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    auto castMessage = (SpaData<float>*)message;
    float payload(castMessage->payload);

    std::cout << "Payload from " << message->spaHeader.source << ":" << payload << std::endl;

    if(message->spaHeader.source == la_FILTER && inRange(payload) ||
       message->spaHeader.source == la_RADIO && payload == 1.0)
    {
      curMessage = "DEPLOYING BOOM";
      deploy();
      curMessage = "BOOM DEPLOYED";
    }
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = curMessage;
    std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    /* Init GPIO */
	  // May need to use echo to do the same here...
    export_file = fopen("/sys/class/gpio/export", "w");
    fwrite(str.c_str(), 1, sizeof(str.c_str()), export_file);
    fclose(export_file);
    
    /* Subscribe to components */
    subscribe(la_FILTER);
    subscribe(la_RADIO);
  }

private:

  bool inRange(float val)
  {
    return (val >= 30 && val <= 44);
  }

  void deploy()
  {
    IO_direction = fopen ("/sys/class/gpio/gpio68/direction", "w");
    fwrite(str2.c_str(), 1, sizeof(str1.c_str()), IO_direction);   //set the pin to HIGH
    fclose(IO_direction);
    sleep(3);
    IO_direction = fopen("/sys/class/gpio/gpio68/direction", "w");
    fwrite(str1.c_str(), 1, sizeof(str1.c_str()), IO_direction);   //set the pin to LOW
    fclose(IO_direction);
    sleep(3);
  }

  FILE* export_file = nullptr;
  FILE* IO_direction = nullptr;

  std::string str1 = "low";
  std::string str2 = "high";
  std::string str = "68";

  std::string curMessage = "Awaiting deployment...";
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}

