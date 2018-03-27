#include "../addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"
#include <stdlib.h>

/** \brief Change "COMPONENT_NAME" and "la_COMPONENT_NAME" to "YOUR_COMPONENT" and "la_YOUR_COMPONENT".
 * #define COMP_NAME represents the name of the component.
 * #define COMP_ADDR represents the logical address of the component.
 */
#define COMP_NAME COMPONENT_NAME
#define COMP_ADDR la_COMPONENT_NAME
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
  }

  void sendData(LogicalAddress destination)
  {
    pResult = PyObject_CallFunction(pFunc, NULL);
    double payload = PyFloat_AsDouble(pResult);
    sendPayload(payload, destination);
  }

  void init()
  {
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyRun_SimpleString("import py_component");

    pName = PyString_FromString("py_component");
    pModule = PyImport_Import(pName);
    if (pModule == NULL)
    {
      std::cout << "Null Module!" << std::endl;
      PyErr_Print();
    }
    pDict = PyModule_GetDict(pModule);

    pFunc = PyDict_GetItemString(pDict, "init");

    //not capturing result, should inits return anything?
    PyObject_CallFunction(pFunc, NULL);
    std::cout << "py_component initialized" << std::endl;

    pFunc = PyDict_GetItemString(pDict, "sendData");
  }
};

int main()
{
  component_start<COMP_NAME>(COMP_ADDR);
  return EXIT_SUCCESS;
}
