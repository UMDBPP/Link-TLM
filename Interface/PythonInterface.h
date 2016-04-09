/* Link Telemetry v0.2.0 "Columbia"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-07-14

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

#include <string>

namespace BPP {

    // Static classes are bad practice in C++, but so are loose functions.
    // The solution is to use a namespace with ONLY the "loose" functions.
    namespace PythonInterface {

        // Hand a Python function a string, get a string back.
        // Arguments: Python source file name (without extension), Python function name, string argument, install directory.
        std::string getString(std::string pySource, std::string pyFunc, std::string argv, std::string directory);

        // Same as above, but returns an integer.
        int getInt(std::string pySource, std::string pyFunc, std::string argv, std::string directory);

        // And again, for floats.
        float getFloat(std::string pySource, std::string pyFunc, std::string argv, std::string directory);

    } // PythonInterface

} // BPP

#endif
