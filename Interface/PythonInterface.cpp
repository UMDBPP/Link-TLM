/* Link Telemetry v0.1.0 "Charlie Brown"
   
   Copyright (c) 2015 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-07-15

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   PythonInterface.cpp:

   Definitions for the Python interface functions declared in PythonInterface.h.
*/

#include "PythonInterface.h"

#include <Python.h> // Home to the Python interface library. Python version 2.7

std::string BPP::PythonInterface::getString(std::string pySource, std::string pyFunc, std::string argv, std::string directory) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.

    // Get location of Python scripts:
    std::string pythonDir = "sys.path.append(\"" + directory + "Python\")";

    // Initialize Python:
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString(pythonDir.c_str());
    
    pName = PyString_FromString(pySource.c_str()); // Python-ify the name of the source file.

    pModule = PyImport_Import(pName); // Import the python source.

    pArgs = PyTuple_New(1); // Set the args (must be in a tuple.)
    PyTuple_SetItem(pArgs, 0, PyString_FromString(argv.c_str()));

    pFunc = PyObject_GetAttrString(pModule, pyFunc.c_str()); // Grab the function specified by the C++ function call.
    if (PyCallable_Check(pFunc)) { // Ensure the function is callable, then call it.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Extract the return value:
    std::string result(PyString_AsString(pValue));

    // Dereference the objects.
    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);

    // Finish the Python Interpreter
    Py_Finalize();

    return result;
}

int BPP::PythonInterface::getInt(std::string pySource, std::string pyFunc, std::string argv, std::string directory) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.

    // Get location of Python scripts:
    std::string pythonDir = "sys.path.append(\"" + directory + "Python\")";

    // Initialize Python:
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString(pythonDir.c_str());
    
    pName = PyString_FromString(pySource.c_str()); // Python-ify the name of the source file.

    pModule = PyImport_Import(pName); // Import the python source.

    pArgs = PyTuple_New(1); // Set the args (must be in a tuple.)
    PyTuple_SetItem(pArgs, 0, PyString_FromString(argv.c_str()));

    pFunc = PyObject_GetAttrString(pModule, pyFunc.c_str()); // Grab the function specified by the C++ function call.
    if (PyCallable_Check(pFunc)) { // Ensure the function is callable, then call it.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Extract the return value:
    int result(PyInt_AsLong(pValue));

    // Dereference the objects.
    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);

    // Finish the Python Interpreter
    Py_Finalize();

    return result;
}

float BPP::PythonInterface::getFloat(std::string pySource, std::string pyFunc, std::string argv, std::string directory) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.

    // Initialize Python:
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append(\"/home/nick/Desktop/Link_TLM/Python\")");
    
    pName = PyString_FromString(pySource.c_str()); // Python-ify the name of the source file.

    pModule = PyImport_Import(pName); // Import the python source.

    pArgs = PyTuple_New(1); // Set the args (must be in a tuple.)
    PyTuple_SetItem(pArgs, 0, PyString_FromString(argv.c_str()));

    pFunc = PyObject_GetAttrString(pModule, pyFunc.c_str()); // Grab the function specified by the C++ function call.
    if (PyCallable_Check(pFunc)) { // Ensure the function is callable, then call it.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Extract the return value:
    float result(PyFloat_AsDouble(pValue));

    // Dereference the objects.
    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);

    // Finish the Python Interpreter
    Py_Finalize();

    return result;
}