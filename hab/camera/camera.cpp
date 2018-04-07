#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"
#include <stdlib.h>

#define COMP_NAME Camera
#define COMP_ADDR la_CAMERA
#define MNGR_ADDR la_LSM

class COMP_NAME : public Component
{
private:
  PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue, *pResult;

public:
  COMP_NAME(std::shared_ptr<LocalCommunicator> com = nullptr) : Component(com, COMP_ADDR, MNGR_ADDR)
  {
  }

  void handleSpaData(SpaMessage* message)
  {
    //Py_Initialize();
    //PyRun_SimpleString("import sys; sys.path.append('.')");
    //PyRun_SimpleString("import py_component");

    auto castMessage = (SpaString*)message;
    std::string payload(castMessage->st);

    std::cout << "Payload: " << payload << std::endl;

    pFunc = PyDict_GetItemString(pDict, "handleSpaData");		
    char* data = const_cast<char*>(payload.c_str()); // since c_str() returns a const char*
    PyObject_CallFunction(pFunc, data);
  }

  void sendData(LogicalAddress destination)
  {
    //Py_Initialize();
    //PyRun_SimpleString("import sys; sys.path.append('.')");
    //PyRun_SimpleString("import py_component");

    // PYTHON FILE SENDDATA
    pFunc = PyDict_GetItemString(pDict, "sendData");
	
    sleep(1);
    pResult = PyObject_CallFunction(pFunc, NULL); // pResult is return of func
    std::string payload = PyString_AsString(pResult); // pResult to string
    sendPayload(payload, destination);
  }

  void init()
  {
    // INITIALIZATION
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyRun_SimpleString("import py_component");

    // IMPORT CORRECT COMPONENT PYTHON FILE
    pName = PyString_FromString("py_component");
    pModule = PyImport_Import(pName);
    if (pModule == NULL)
    {
      std::cout << "Null Module!" << std::endl;
      PyErr_Print();
    }

    // GET MODULES FROM COMPONENT PYTHON FILE
    pDict = PyModule_GetDict(pModule);

    // PYTHON FILE INIT
    pFunc = PyDict_GetItemString(pDict, "init");
    // setev("PYTHONPATH", "/usr/lib/python2.7", 1);

    //not capturing result, should inits return anything?
    PyObject_CallFunction(pFunc, NULL);
    std::cout << "py_component initialized" << std::endl;
	
    // SUBSCRIPTIONS
    subscribe(la_BOOM);
    sleep(0.5);
    subscribe(la_RADIO);
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
