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

#ifndef JSONWRITE_H
#define JSONWRITE_H

#include <string>
#include <fstream>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace BPP {

class JSONWriter {

    public:
        JSONWriter(std::string _filename); // In use case, no need for default CTOR.
        ~JSONWriter(); // DTOR, which will actually do something! (close the file.)

        // Overloaded functions to add to JSON object
        void addValue(std::string _key, std::string _value);
        void addValue(std::string _key, int _value);
        void addValue(std::string _key, float _value);

	private:
		std::ofstream logFile;
        bool logOpen;

        rapidjson::StringBuffer buf;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer;

}; // JSONWriter

} // BPP

#endif // JSONWRITE_H
