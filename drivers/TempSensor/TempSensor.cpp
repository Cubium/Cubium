#include "../demo_addresses.hpp"
#include <component.hpp>
#include <iostream>
#include <unistd.h>

#include "python2.7/Python.h"

#define COMP_NAME TempSensor
#define COMP_ADDR la_TempSensor
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

// Initialize the Python interpreter.
setenv("PYTHONPATH","./Adafruit_Python_MCP9808/Adafruit_MCP9808",1);
Py_Initialize();
/*
POSSIBLY THIS TOO
  setArgv(argc, argv); // must call this to get sys.argv and relative imports
  PyRun_SimpleString("import os, sys\n"
                     "print sys.argv, \"\\n\".join(sys.path)\n"
                     "print os.getcwd()\n"
                     "import thing\n" // import a relative module
                     "thing.printer()\n");
  Py_Finalize();
 */

// Create some Python objects that will later be assigned values.

PyObject *pName, *pModule, *pDict, *pClass, *pArgs, *pValue;

// Convert the file name to a Python string.

pName = PyString_FromString("MCP9808");

if(pName==NULL) std::cout << "NULL name!" << std::endl;

// Import the file as a Python module.

pModule = PyImport_Import(pName);

if(pModule==NULL) 
{
	std::cout << "NULL module!" << std::endl;
	PyErr_Print();
}

// Create a dictionary for the contents of the module.

//PySys_SetArgv(argc,argv);
pDict = PyModule_GetDict(pModule);


// Get the add method from the dictionary.

pClass = PyObject_GetAttrString(pModule,(char*)"MCP9808");
//pFunc = PyDict_GetItemString(pDict, "pythonFunc");

if(pClass==NULL)
{
	PyErr_Print();
	std::cout << "NULL class!" << std::endl;
}

//PyObject* instance = PyInstance_New(pClass, NULL, NULL);
PyObject* instance = PyObject_CallObject(pClass, NULL);
if(instance==NULL)
{
	PyErr_Print();
	std::cout << "NULL instance!" << std::endl;
}

// Call Python class method
pName = PyString_FromString("begin");

PyObject* pResult = PyObject_CallMethodObjArgs(instance, pName, NULL);
if(pResult==NULL)
{
	PyErr_Print();
}
pName = PyString_FromString("readTempC");

pResult = PyObject_CallMethodObjArgs(instance, pName, NULL);
//PyObject* pResult = PyObject_Call(PyObject_GetAttrString(instance, "readTempC"), PyTuple_New(0), pDict);
//PyObject* pResult = PyObject_CallMethod(instance, "begin", NULL);
/*
if(pFunc==NULL)
{
	PyErr_Print();
	std::cout << "NULL function!" << std::endl;
}
*/

// Create a Python tuple to hold the arguments to the method.

//pArgs = PyTuple_New(1);

// Convert 2 to a Python integer.

//pValue = PyInt_FromLong(2);

// Set the Python int as the first and second arguments to the method.

//PyTuple_SetItem(pArgs, 0, pValue);


// Call the function with the arguments.

//PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

// Print a message if calling the method failed.

if(pResult == NULL)

printf("Calling the method failed.\n");

// Convert the result to a long from a Python object.

long result = PyInt_AsLong(pResult);

// Destroy the Python interpreter.

Py_Finalize();

// Print the result.

printf("The result is %d.\n", result); 
    sleep(1);
    std::string payload = "You can send over anything you want! Anything at all!";

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
