#include "../addresses.hpp"
#include <component.hpp>
#include <functional>
#include <iostream>
#include <numeric>
#include <chrono>
#include <thread>

#define COMP_NAME Aggregator
#define COMP_ADDR la_AGGREGATOR
#define MNGR_ADDR la_LSM

const auto one_sec = std::chrono::milliseconds(1000);
const auto half_sec = std::chrono::milliseconds(500);

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
    std::this_thread::sleep_for(one_sec);
    data[0] = getTime();
    std::string payload = serialize(data);
    //std::cout << "Sending data: " << payload << std::endl;
    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_BOOM);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_FILTER);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_BAROMETER);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_BATTERY);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_DIGITAL_TEMP);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_ANALOG_TEMP1);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_ANALOG_TEMP2);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_UV_INTERNAL);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_UV_EXTERNAL);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_GYROSCOPE);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_MAGNETOMETER);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_LIGHT_INTERNAL);
    std::this_thread::sleep_for(half_sec);
    subscribe(la_LIGHT_EXTERNAL);
    std::this_thread::sleep_for(half_sec);
  } 

private:
  std::vector<std::string> data = {"", "", "", "", "", "", "", "", "", "", "", "", "", ""};

  std::map<LogicalAddress, std::function<void(SpaMessage*)>> handler =
      {
          {la_BAROMETER,      [this](SpaMessage* m) { data[1]  = extractPayloadFloat(m);  }},
          {la_FILTER,         [this](SpaMessage* m) { data[2]  = extractPayloadFloat(m);  }},
          {la_BOOM,           [this](SpaMessage* m) { data[3]  = extractPayloadString(m); }},
          {la_BATTERY,        [this](SpaMessage* m) { data[4]  = extractPayloadString(m);  }},
          {la_DIGITAL_TEMP,   [this](SpaMessage* m) { data[5]  = extractPayloadFloat(m);  }},
          {la_ANALOG_TEMP1,   [this](SpaMessage* m) { data[6]  = extractPayloadFloat(m);  }},
          {la_ANALOG_TEMP2,   [this](SpaMessage* m) { data[7]  = extractPayloadFloat(m);  }},
          {la_UV_INTERNAL,    [this](SpaMessage* m) { data[8]  = extractPayloadFloat(m);  }},
          {la_UV_EXTERNAL,    [this](SpaMessage* m) { data[9]  = extractPayloadFloat(m);  }},
          {la_GYROSCOPE,      [this](SpaMessage* m) { data[10] = extractPayloadString(m); }},
          {la_MAGNETOMETER,   [this](SpaMessage* m) { data[11] = extractPayloadString(m); }},
          {la_LIGHT_INTERNAL, [this](SpaMessage* m) { data[12] = extractPayloadFloat(m);  }},
          {la_LIGHT_EXTERNAL, [this](SpaMessage* m) { data[13] = extractPayloadFloat(m);  }},
      };


  std::string getTime()
  {
    return std::to_string(
        std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now()
        )
    );
  }


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
