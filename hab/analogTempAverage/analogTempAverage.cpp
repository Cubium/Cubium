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
  float wallAverage = 0.0;
  float floorAverage = 0.0;

public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    
    if (message->spaHeader.source == la_ANALOG_TEMP_WALL)
    {
	auto castMessage = (SpaData<float>*)message;
	float payloadFloat(castMessage->payload);
	wallAverage = payloadFloat;	
    }
    if (message->spaHeader.source == la_ANALOG_TEMP_FLOOR)
    {
	auto castMessage = (SpaData<float>*)message;
	float payloadFloat(castMessage->payload);
	floorAverage = payloadFloat;	
    }
	average = (wallAverage + floorAverage)/2; 
  }

  void sendData(LogicalAddress destination)
  {
    std::cout << average << std::endl;
    sendPayload(average, destination);
  }

  void init()
  {
    subscribe(la_ANALOG_TEMP_WALL);
    sleep(1.0);
    subscribe(la_ANALOG_TEMP_FLOOR);
    sleep(1.0);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
