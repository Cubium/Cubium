#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"
#include <stdlib.h>

#define COMP_NAME ANALOG_TEMP_AVERAGE
#define COMP_ADDR la_ANALOG_TEMP_AVERAGE
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
private:
  float average = 0.0;

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

  void sendData(LogicalAddress destination)
  {
    //pResult = PyObject_CallFunction(pFunc, NULL);
    //float payload = PyFloat_AsDouble(pResult);
    sendPayload(average, destination);
  }

  void init()
  {
    subscribe(la_ANALOG_TEMP_WALL);
    sleep(0.5);
    subscribe(la_ANALOG_TEMP_FLOOR);
    sleep(0.5);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
