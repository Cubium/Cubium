#include "../demo_addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"
#include <stdlib.h>

#define COMP_NAME CompA
#define COMP_ADDR la_PYC
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
public:
  COMP_NAME(std::shared_ptr<SpaCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {

    std::cout << "Payload: Got it \n"; //" << message->payload << std::endl;
  }

  void sendData(LogicalAddress destination)
  {
    sleep(1);

    //setenv("PYTHONPATH","./",1);

    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyRun_SimpleString("import rando");

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue, *pResult;

    pName = PyString_FromString("rando");
    pModule = PyImport_Import(pName);
    if (pModule == NULL)
    {
      std::cout << "Null Module!" << std::endl;
      PyErr_Print();
    }
    pDict = PyModule_GetDict(pModule);

    pFunc = PyDict_GetItemString(pDict, "getRand");
    //setenv("PYTHONPATH","/usr/lib/python2.7",1);

    pResult = PyObject_CallFunction(pFunc, NULL);

    int payload = PyInt_AsLong(pResult);

    std::cout << "Sending SpaData: " << payload << std::endl;

    sendPayload(payload, destination);
  }

  void init()
  {
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
