#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <unistd.h>

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
    if (message->spaHeader.source == la_RADIO)
    {
      auto payload(((SpaString*)message)->st);
      if (isDeploy(payload))
      {
        //std::cout << "DEPLOYING!\n";
        curMessage = "DEPLOYING BOOM";
        curMessage = "BOOM DEPLOYED";
      }
    }
    else if (message->spaHeader.source == la_FILTER)
    {
      float payload = ((SpaData<float>*)message)->payload;
      if (inRange(payload))
      {
        //std::cout << "DEPLOYING!\n";
        curMessage = "DEPLOYING BOOM";
        curMessage = "BOOM DEPLOYED";
      } 
    }
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = curMessage;
    //std::cout << "Sending payload: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    /* Init GPIO */
    // May need to use echo to do the same here...
    //    export_file = fopen("/sys/class/gpio/export", "w");
    //    fwrite(str.c_str(), 1, sizeof(str.c_str()), export_file);
    //    fclose(export_file);

    /* Subscribe to components */
    subscribe(la_RADIO);
    subscribe(la_FILTER);
  }

private:

  std::string d = "deploy";

  bool isDeploy(std::string p)
  {
    for (auto i = 0u; i < d.size(); ++i)
    {
      if (p[i] != d[i])
      {
        return false;
      }
    }
    return true;
  }

  bool inRange(float val)
  {
    return (val >= 30 && val <= 44);
  }

  void deploy()
  {
    IO_direction = fopen("/sys/class/gpio/gpio68/direction", "w");
    fwrite(str2.c_str(), sizeof(char), strlen(str2.c_str()), IO_direction); //set the pin to HIGH
    fclose(IO_direction);
    sleep(3);
    IO_direction = fopen("/sys/class/gpio/gpio68/direction", "w");
    fwrite(str1.c_str(), sizeof(char), strlen(str1.c_str()), IO_direction); //set the pin to LOW
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
