/*  Link Telemetry v1.0 "Odyssey"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2015-10-31

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

    Log.h:

    Simple text file logging class. Should clean up packet logging efforts.
    Uses C++ templates to deal with different argument types and numbers of arguments.
*/

#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>

namespace BPP {

class Log {

    private:
        std::ofstream logfile; // File object
        bool logOpen; // Sanity check

        std::string getTS(); // Get a stringified timestamp

    public:
        Log(); // CTOR
        ~Log(); // DTOR which will close file (not empty).

        bool open(std::string filename); // Initializer to open log file.

        // Logging function overloads
        void log();

        // Templates must be in header:

        // Only one argument given/left to log.
        template<typename T> void log(const T& t) { // This construct allows any type.
            if(logOpen) { // Check to make sure file is open.
                logfile << t << ","; // Write out argument.
                log(); // Recursive call to empty log function, for timestamp.
            }
        }

        // More than one argument given to logging function.
        // Gets called until only one argument left, then moves to above overload.
        template<typename F,typename ...T> void log(const F& first, const T&... rest) { // Construct allows any type/number of args.
            if(logOpen) { // Check to make sure file is open.
                logfile << first << ","; // Write out first argument.
                log(rest...); // Recursive call on rest of argument.
            }
        }

};

}

#endif
// LOG_H