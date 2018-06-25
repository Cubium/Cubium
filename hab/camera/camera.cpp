#include "../addresses.hpp"
#include "../parameters.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"

#define COMP_NAME Camera
#define COMP_ADDR la_CAMERA
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    unsigned delay = 0, num_pictures = 0;

    if (message->spaHeader.source == la_BOOM)
    {
      num_pictures = 3;
      delay = 3;
    }

    else if (message->spaHeader.source == la_RADIO)
    {
      auto message_str = (SpaString*)message;
      std::string payload(message_str->st);

      if (payload == RADIO_PICTURE_COMMAND)
      {
        num_pictures = 1;
        delay = 1;
      }
    }

    for (auto i = 0; i < num_pictures; ++i)
    {
      PyRun_SimpleString("import sys; sys.path.append('.')");
      PyRun_SimpleString("import py_component");

      PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue, *pResult;

      pName = PyString_FromString("py_component");
      pModule = PyImport_Import(pName);

      if (pModule == NULL)
      {
        std::cout << "Null Module!" << std::endl;
        PyErr_Print();
      }

      pDict = PyModule_GetDict(pModule);
      pFunc = PyDict_GetItemString(pDict, "handleSpaData");
      pResult = PyObject_CallFunction(pFunc, NULL);

      sleep(delay);
    }
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyRun_SimpleString("import py_component");

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue, *pResult;

    pName = PyString_FromString("py_component");
    pModule = PyImport_Import(pName);

    if (pModule == NULL)
    {
      std::cout << "Null Module!" << std::endl;
      PyErr_Print();
    }

    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, "sendData");
    pResult = PyObject_CallFunction(pFunc, NULL);

    auto cStr = PyString_AsString(pResult);
    std::string payload(cStr);

    sendPayload(payload, destination);
  }

  void init()
  {
    subscribe(la_BOOM);
    sleep(1);
    subscribe(la_RADIO);

    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyRun_SimpleString("import py_component");

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue, *pResult;

    pName = PyString_FromString("py_component");
    pModule = PyImport_Import(pName);

    if (pModule == NULL)
    {
      std::cout << "Null Module!" << std::endl;
      PyErr_Print();
    }

    pDict = PyModule_GetDict(pModule);
    pFunc = PyDict_GetItemString(pDict, "init");

    PyObject_CallFunction(pFunc, NULL);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
