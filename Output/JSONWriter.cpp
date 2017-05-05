/*  Link Telemetry v1.0 "Odyssey"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2017-05-04

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

    JSONWriter.h:

    Wrapper for RapidJSON PrettyWriter class which also handles file I/O.
*/

#include "JSONWriter.h"

#include <iostream>

// Constructor
// Due to class use case, defualt constructor is not required.
// Takes in filename for JSON file, opens and error checks.
BPP::JSONWriter::JSONWriter(std::string _filename) : writer(buf) {
    logFile.open(_filename, std::ofstream::out | std::ofstream::app); // Open file
    logOpen = logFile.is_open(); // Set file open status

    writer.StartObject(); // Open up the JSON object
}

// Destructor
// Actually does something!
// Finshes the JSON object, writes the formatted object to the file, then closes the file.
BPP::JSONWriter::~JSONWriter() {
    writer.EndObject(); // Close the JSON object

    if(logOpen) {
        logFile << buf.GetString() << std::endl; // Write out JSON-ified string to file
        logFile.close(); // Clean up, close file.
    } else {
        std::cerr << "WARNING: JSON output not opened; writing to file failed!\n";
    }
}

// Functions to add key-val pairs to JSON object.
// Overloadedd for string, int, float.
// Arrays not handled (will add if needed.)
void BPP::JSONWriter::addValue(std::string _key, std::string _value) {
    writer.Key(_key.c_str());
    writer.String(_value.c_str());
}

void BPP::JSONWriter::addValue(std::string _key, int _value) {
    writer.Key(_key.c_str());
    writer.Int(_value);
}

void BPP::JSONWriter::addValue(std::string _key, float _value) {
    writer.Key(_key.c_str());
    writer.Double(_value); // Note JSON only supports doubles, not floats. Eh.
}
