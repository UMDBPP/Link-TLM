/*  Link Telemetry v1.0 "Odyssey"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2015-10-30

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

    JSONLoader.h:

    Load JSON settings file. Uses RapidJson json parsing library (see deps/rapidjson)
    This is kind of a single use class, but it's better than a singleton.

    Settings in file:
    - Balloon Callsigns
    - Van Callsigns
    - Log File Names
*/

#ifndef JSONLOAD_H
#define JSONLOAD_H

#include <string>
#include <vector>

#include <rapidjson/document.h>

namespace BPP {

class JSONLoader {

    private:
        rapidjson::Document settings; // JSON DOM for settings file.
        bool loadFailure; // Records JSON failure status.
        std::string jsonFilename; // Store filename for error reporting.

    public:
        JSONLoader(const std::string& _jsonFileName); // Construct class with a filename.
        ~JSONLoader(); // DTOR - Probably trivial.

        std::vector<std::string> getStringVector(const std::string& _jsonMember);
        std::string getString(const std::string& _jsonMember);

};

} // BPP

#endif
// JSONLOAD_H