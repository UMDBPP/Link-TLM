/*  Link Telemetry v0.1.0 "Charlie Brown"
    
    Copyright (c) 2015 University of Maryland Space Systems Lab
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

    Log.cpp:

    Simple text file logging class. Should clean up packet logging efforts...
*/

#include "Log.h"

#include <ctime>

BPP::Log::Log() {
    logOpen = false; // The file isn't open on construction.
}

BPP::Log::~Log() {
    logfile.close(); // Clean up.
}

// Get a timestamp
std::string BPP::Log::getTS() {
    time_t raw;
    struct tm *t;

    std::time(&raw); // get time in seconds
    t = std::localtime(&raw); // offset for local time and convert to hours, min, sec

    return std::asctime(t); // Turn TS into a string
}

// Initializer which opens the specified log file.
// Opens in append to not wipe data.
// Drops a timestamp to signify when log started.
bool BPP::Log::open(std::string filename) {
    logfile.open(filename, std::ofstream::out | std::ofstream::app); // Open the file.
    logOpen = logfile.is_open(); // Set open status.

    if(logOpen) {
        logfile << "LOG BEGINNING ON " << getTS() << std::endl; // Drop timestamp and log beginning.
    }

    return logOpen; // Allows failure checking.
}

// No arguments given/left to log
// Therefore header-defined template isn't necessary.
// Gets called after all arguments expended; places timestamp.
void BPP::Log::log() {
    std::string ts = getTS(); // Grab timestamp.
    ts.erase(ts.length()-1); // Strip newline.

    if(logOpen) {
        logfile << "  < LOGGED AT " << ts << " >" << std::endl; // Drop the timestamp and end line.
    }
}
