#include "../addresses.hpp"
#include <component.hpp>
#include <functional>
#include <iostream>
#include <numeric>
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
    handler[message->spaHeader.source](message);
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    std::string payload = serialize(data);
    std::cout << "Sending data: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_BOOM);
    sleep(0.5);
    subscribe(la_FILTER);
    sleep(0.5);
    subscribe(la_BAROMETER);
    sleep(0.5);
    subscribe(la_BATTERY);
    sleep(0.5);
    subscribe(la_DIGITAL_TEMP);
    sleep(0.5);
    subscribe(la_ANALOG_TEMP1);
    sleep(0.5);
    subscribe(la_ANALOG_TEMP2);
    sleep(0.5);
    subscribe(la_UV_INTERNAL);
    sleep(0.5);
    subscribe(la_UV_EXTERNAL);
    sleep(0.5);
    subscribe(la_GYROSCOPE);
    sleep(0.5);
    subscribe(la_MAGNETOMETER);
    sleep(0.5);
    subscribe(la_RTC);
    sleep(0.5);
    subscribe(la_LIGHT_INTERNAL);
    sleep(0.5);
    subscribe(la_LIGHT_EXTERNAL);
    sleep(0.5);
  } 

private:
  std::vector<std::string> data = {"", "", "", "", "", "", "", "", "", "", "", "", "", ""};

  std::map<LogicalAddress, std::function<void(SpaMessage*)>> handler =
      {
          {la_RTC,            [this](SpaMessage* m) { data[0]  = extractPayloadString(m); }},
          {la_BAROMETER,      [this](SpaMessage* m) { data[1]  = extractPayloadFloat(m);  }},
          {la_FILTER,         [this](SpaMessage* m) { data[2]  = extractPayloadFloat(m);  }},
          {la_BOOM,           [this](SpaMessage* m) { data[3]  = extractPayloadString(m); }},
          {la_BATTERY,        [this](SpaMessage* m) { data[4]  = extractPayloadString(m);  }},
          {la_DIGITAL_TEMP,   [this](SpaMessage* m) { data[5]  = extractPayloadString(m);  }},
          {la_ANALOG_TEMP1,   [this](SpaMessage* m) { data[6]  = extractPayloadFloat(m);  }},
          {la_ANALOG_TEMP2,   [this](SpaMessage* m) { data[7]  = extractPayloadFloat(m);  }},
          {la_UV_INTERNAL,    [this](SpaMessage* m) { data[8]  = extractPayloadString(m);  }},
          {la_UV_EXTERNAL,    [this](SpaMessage* m) { data[9]  = extractPayloadString(m);  }},
          {la_GYROSCOPE,      [this](SpaMessage* m) { data[10] = extractPayloadString(m); }},
          {la_MAGNETOMETER,   [this](SpaMessage* m) { data[11] = extractPayloadString(m); }},
          {la_LIGHT_INTERNAL, [this](SpaMessage* m) { data[12] = extractPayloadString(m);  }},
          {la_LIGHT_EXTERNAL, [this](SpaMessage* m) { data[13] = extractPayloadString(m);  }},
      };


  std::string extractPayloadString(SpaMessage* message)
  {
    return ((SpaString*)message)->st;
  }

  std::string extractPayloadFloat(SpaMessage* message)
  {
    return std::to_string(((SpaData<float>*)message)->payload);
  }

  /* Take a vector of strings and return a string of each
     element delimited by ';'                          */
  std::string serialize(std::vector<std::string> v)
  {
    return std::accumulate(std::next(v.begin()),
                           v.end(),
                           v[0],
                           [](std::string a, std::string b) {
                             return a + ';' + b;
                           });
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
