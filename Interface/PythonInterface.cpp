/* Link Telemetry v0.3 "Yankee Clipper"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-07-15
   2016-04-15

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

// Start up the Python interpreter.
// Also, set the directory path so Python can access relevant code.
void BPP::PythonInterface::initPython(std::string directory) {
    Py_Initialize(); // Actually start
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("import os"); // Bring in libraries to alter path.

    // Get full directory path:
    std::string pythonDirCmd = "sys.path.append(\"" + directory + "Python\")";
    PyRun_SimpleString(pythonDirCmd.c_str()); // Append directory to path.
}

// Shut down the Python interpreter.
// It's simple but this needs to be external.
void BPP::PythonInterface::stopPython() {
    Py_Finalize();
}

std::string BPP::PythonInterface::getString(std::string pySource, std::string pyFunc, std::string argv) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.
    
    pName = PyString_FromString(pySource.c_str()); // Python-ify the name of the source file.

    pModule = PyImport_Import(pName); // Import the python source.

    pArgs = PyTuple_New(1); // Set the args (must be in a tuple.)
    PyTuple_SetItem(pArgs, 0, PyString_FromString(argv.c_str()));

    pFunc = PyObject_GetAttrString(pModule, pyFunc.c_str()); // Grab the function specified by the C++ function call.
    if (PyCallable_Check(pFunc)) { // Ensure the function is callable, then call it.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print(); // If an error occurs, print it.
    }

    // Extract the return value:
    std::string result(PyString_AsString(pValue));

    // Dereference the objects.
    Py_DECREF(pModule);
    Py_DECREF(pName);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);
    Py_XDECREF(pFunc);

    return result;
}

int BPP::PythonInterface::getInt(std::string pySource, std::string pyFunc, std::string argv) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.
    
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
    Py_XDECREF(pFunc);

    return result;
}

float BPP::PythonInterface::getFloat(std::string pySource, std::string pyFunc, std::string argv) {
    PyObject *pName, *pModule, *pArgs, *pFunc, *pValue = NULL; // Create Python objects.
    
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
    Py_XDECREF(pFunc);

    return result;
}

// Start the matplotlib plotting interface.
void BPP::PythonInterface::initPlotting() {
    PyObject *pName, *pModule, *pFunc; // Python objects.

    std::string pySource = "BPPplotter";
    pName = PyString_FromString(pySource.c_str());
    pModule = PyImport_Import(pName); // Get python script name and import it.

    std::string pyFunction = "initInteractivePlot";
    pFunc = PyObject_GetAttrString(pModule, pyFunction.c_str()); // Get function object for init func.
    if(PyCallable_Check(pFunc)) { // Ensure function is actually available
        PyObject_CallObject(pFunc, NULL); // Init function takes no args.
    } else {
        PyErr_Print(); // If function wasn't callable, there was an error. Print it.
    }

    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
}

// Create a plot.
// Also titles the plot and labels the axes.
// Default to empty values for labels.
int BPP::PythonInterface::createPlot(std::string _title, std::string _xLabel, std::string _yLabel) {
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue = NULL; // Create all the Python objects we'll need.

    std::string pySource = "BPPplotter";
    pName = PyString_FromString(pySource.c_str());
    pModule = PyImport_Import(pName); // Import the plotting script.

    // Set the arguments:
    pArgs = PyTuple_New(3);
    PyTuple_SetItem(pArgs, 0, PyString_FromString(_title.c_str()));
    PyTuple_SetItem(pArgs, 1, PyString_FromString(_xLabel.c_str()));
    PyTuple_SetItem(pArgs, 2, PyString_FromString(_yLabel.c_str()));

    std::string pyFunction = "createPlot";
    pFunc = PyObject_GetAttrString(pModule, pyFunction.c_str()); // Get function object.
    if(PyCallable_Check(pFunc)) { // Call the function if it's callable, error if not.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    int plotHandle(PyInt_AsLong(pValue)); // Retrieve the plot handle int from Python val.

    // Decrement references.
    // Thanks to Python's wonky memory management.
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);

    return plotHandle; // Give user handle to created plot.
}

// Create subplots on an initialized plot.
// Can handle up to four - don't think we'll even need that many.
// Returns vector of subplot handles, with index 0 as the first subplot and back() as the last.
std::vector<int> BPP::PythonInterface::createSubplots(int _plotHandle, int _numSubPlots) {
    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue = NULL; // Create all the Python objects we'll need.

    std::string pySource = "BPPplotter";
    pName = PyString_FromString(pySource.c_str());
    pModule = PyImport_Import(pName); // Import the plotting script.

    // Set arguments:
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, PyInt_FromLong(_plotHandle));
    PyTuple_SetItem(pArgs, 1, PyInt_FromLong(_numSubPlots));

    std::string pyFunction = "createSubplots";
    pFunc = PyObject_GetAttrString(pModule, pyFunction.c_str()); // Get function object.
    if(PyCallable_Check(pFunc)) { // Call the function if it's callable, error if not.
        pValue = PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Decrement references.
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pValue);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);

    // Create vector of possible subplot handles depending on number of requested plots.
    // Don't want to handle passing an array back from Python, but I already know exactly what these will be.
    // Two and three subplots are extra rows.
    // Four is two rows, two cols.
    // Anything else makes just one.
    std::vector<int> subplotHandles;
    switch(_numSubPlots) {
        case 2:
            subplotHandles.push_back(211);
            subplotHandles.push_back(212);
            break;
        case 3:
            subplotHandles.push_back(311);
            subplotHandles.push_back(312);
            subplotHandles.push_back(313);
            break;
        case 4:
            subplotHandles.push_back(221);
            subplotHandles.push_back(222);
            subplotHandles.push_back(223);
            subplotHandles.push_back(224);
            break;
        default:
            subplotHandles.push_back(111);
            break;
    }

    return subplotHandles;
}

// Format a specific subplot with a title and axis labels.
// This will only format one subplot on a graph, not all of them.
// Call once for each subplot. (Or don't, and have unlabelled graphs!)
void BPP::PythonInterface::formatSubplot(int _plotHandle, int _subplotHandle, std::string _title, std::string _xLabel, std::string _yLabel) {
    PyObject *pName, *pModule, *pFunc, *pArgs; // Create needed Python objects

    std::string pySource = "BPPplotter";
    pName = PyString_FromString(pySource.c_str());
    pModule = PyImport_Import(pName); // Import the plotting script.

    // Set arguments:
    pArgs = PyTuple_New(5);
    PyTuple_SetItem(pArgs, 0, PyInt_FromLong(_plotHandle));
    PyTuple_SetItem(pArgs, 1, PyInt_FromLong(_subplotHandle));
    PyTuple_SetItem(pArgs, 2, PyString_FromString(_title.c_str()));
    PyTuple_SetItem(pArgs, 3, PyString_FromString(_xLabel.c_str()));
    PyTuple_SetItem(pArgs, 4, PyString_FromString(_yLabel.c_str()));

    std::string pyFunction = "formatSubplot";
    pFunc = PyObject_GetAttrString(pModule, pyFunction.c_str()); // Get function object.
    if(PyCallable_Check(pFunc)) { // Call the function if it's callable, error if not.
        PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Decrement references.
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);
}

// Overload of plotting function for graphs without subplots.
void BPP::PythonInterface::plotDataPoint(int _plotHandle, float _x1, float _y1, float _x2, float _y2) {
    plotDataPoint(_plotHandle, 111, _x1, _y1, _x2, _y2); // Simply call the full version with the default subplot arg.
}

// Plot data on the graph. Select for subplot.
// x1,y1 is a previous data point, x2,y2 is the current.
// This setup allows plotting a continuous line instead of single points.
void BPP::PythonInterface::plotDataPoint(int _plotHandle, int _subplotHandle, float _x1, float _y1, float _x2, float _y2) {
    PyObject *pName, *pModule, *pFunc, *pArgs;

    std::string pySource = "BPPplotter";
    pName = PyString_FromString(pySource.c_str());
    pModule = PyImport_Import(pName); // Import plotting script.

    // Set arguments. Note x/y order!
    // Also, default argument for subplot.
    pArgs = PyTuple_New(6);
    PyTuple_SetItem(pArgs, 0, PyInt_FromLong(_plotHandle));
    PyTuple_SetItem(pArgs, 1, PyInt_FromLong(_subplotHandle));
    PyTuple_SetItem(pArgs, 2, PyFloat_FromDouble(_x1));
    PyTuple_SetItem(pArgs, 3, PyFloat_FromDouble(_x2));
    PyTuple_SetItem(pArgs, 4, PyFloat_FromDouble(_y1));
    PyTuple_SetItem(pArgs, 5, PyFloat_FromDouble(_y2));

    std::string pyFunction = "addPointToPlot";
    pFunc = PyObject_GetAttrString(pModule, pyFunction.c_str()); // Get function
    if(PyCallable_Check(pFunc)) { // Call the function if it's callable, error if not.
        PyObject_CallObject(pFunc, pArgs);
    } else {
        PyErr_Print();
    }

    // Decrement references.
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pFunc);
    Py_DECREF(pArgs);
}
