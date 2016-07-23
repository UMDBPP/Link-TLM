/* Link Telemetry v0.3 "Yankee Clipper"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-07-14
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

   PythonInterface.h:

   Declares the PythonInterface namespace and functions. (Yes, this is a sub-namespace of BPP.)
   Provides a convenient way to get ints, floats, and strings from Python functions.
   Desgined to be generic for expandability, but initial use is for parsing APRS packets.
*/

#ifndef PYTHON_H
#define PYTHON_H

#include <Python.h> // Home to the Python interface library. Python version 2.7
#include <string>
#include <vector>

namespace BPP {

    // Static classes are bad practice in C++, but so are loose functions.
    // The solution is to use a namespace with ONLY the "loose" functions.
    namespace PythonInterface {

        // Initialize the Python interpreter.
        void initPython(std::string directory);

        // Shut down Python.
        void stopPython();

        // Hand a Python function a string, get a string back.
        // Arguments: Python source file name (without extension), Python function name, string argument.
        std::string getString(std::string pySource, std::string pyFunc, std::string argv);

        // Same as above, but returns an integer.
        int getInt(std::string pySource, std::string pyFunc, std::string argv);

        // And again, for floats.
        float getFloat(std::string pySource, std::string pyFunc, std::string argv);

        // Matplotlib wrapper:

        // Initialize matplotlib in interactive mode.
        void initPlotting();

        // Create a plot with given title and axis labels.
        int createPlot(std::string _title = "", std::string _xLabel = "", std::string _yLabel = "");

        // Create subplots on a larger plot.
        std::vector<int> createSubplots(int _plotHandle, int _numSubPlots);

        // Add labels to specified subplot.
        void formatSubplot(int _plotHandle, int _subplotHandle, std::string _title = "", std::string _xLabel = "", std::string _yLabel = "");

        // Actual plotting function, with overload for graphs with subplots.
        // x1,y1 is "old" data point; x2,y2 is "new".
        void plotDataPoint(int _plotHandle, float _x1, float _y1, float _x2, float _y2);
        void plotDataPoint(int _plotHandle, int _subplotHandle, float _x1, float _y1, float _x2, float _y2);

    } // PythonInterface

} // BPP

#endif
