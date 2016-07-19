/*  Link Telemetry v0.3 "Yankee Clipper"
    
    Copyright (c) 2015-2016 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2015-07-08

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

    Parser.cpp:

    Definitions for C++ based parsing to replace Python parsing.
    Implemented as functions behind a namespace, like PythonInterface (which
    will be deprecated in time.)

    Some internal-only functions and structs are declared in this file as well,
    to keep them from having global scope.

    Parser loads regex strings from a JSON file, so they're still easily editable.
*/

#include "Parser.h"

#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "System/JSONLoader.h"

namespace BPP::Parser {

// Structure for the regex defs loaded from config file.
// Only one instance of this is needed, and it isn't needed outside the scope
// of the Parser file.
struct {
    std::regex cs; // Callsign
    std::regex ts; // Timestamp
    std::regex compLatLon;
    std::regex uncompLat;
    std::regex uncompLon;
    std::regex alt; // Altitude
    std::regex cosmpCseSpd; // Course/speed
    std::regex uncompCseSpd;
    std::regex uncompCmt; // Comment
    std::regex compCmt;
} _regexDefs;

} // BPP::Parser

void BPP::Parser::initParsing(std::string _regexFilename) {
    JSONLoader regexFile(_regexFilename); // Load the Json file

    // Store all the regex pattern strings in the internal struct.
    _regexDefs.cs.assign(regexFile.getString("callsignPattern"));
    _regexDefs.ts.assign(regexFile.getString("timestampPattern"));
    _regexDefs.compLatLon.assign(regexFile.getString("compressedLatLonPattern"));
    _regexDefs.uncompLat.assign(regexFile.getString("uncompressedLatPattern"));
    _regexDefs.uncompLon.assing(regexFile.getString("uncompressedLonPattern"));
    _regexDefs.alt.assign(regexFile.getString("altitudePattern"));
    _regexDefs.compCseSpd.assign(regexFile.getString("compressedCseSpdPattern"));
    _regexDefs.uncompCseSpd.assign(regexFile.getString("uncompressedCseSpdPattern"));
    _regexDefs.uncompCmt.assign(regexFile.getString("uncompressedCommentPattern"));
    _regexDefs.compCmt.assign(regexFile.getString("compressedCommentPattern"));
}

std::string BPP::Parser::getCallsign(std::string _rawPacket) {
    return _regexMatch(_rawPacket, _regexDefs.cs); // Simply search for and return the callsign
}

std::string BPP::Parser::getTimestamp(std::string _rawPacket) {
    std::string ts = _regexMatch(_rawPacket, _regexDefs.ts);
    std::string outputTimestamp = "None";

    // Get system time to fill missing details
    auto t = std::time(nullptr);
    auto tmLocal = *std::localtime(&t);
    auto tmUTC = *std::gmtime(&t);

    if(ts != std::string("None")) { // If timestamp was explicit in packet, extract it
        if(str.back() == 'h') { // This is the UTC hour-min-sec format
            std::string hour = ts.substr(0,2);
            std::string minute = ts.substr(2,2);
            std::string second = ts.substr(4,2);

            std::ostringstream date;
            date << std::put_time(&tmUTC, "%Y-%m-%d "); // Extract just the date
            outputTimestamp = date.str() + hour + ":" + minute + ":" + second;
        } else if(str.back() == 'z') { // UTC day-hour-minute format
            std::string day = ts.substr(0,2);
            std::string hour = ts.substr(2,2);
            std::string minute = ts.substr(4,2);

            std::ostringstream ym;
            std::ostringstream second;
            ym << std::put_time(&tmUTC, "%Y-%m-");
            second << std::put_time(&tmUTC, "%S");
            outputTimestamp = ym.str() + day + " " + hour + ":" + minute + ":" + second.str();
        } else if(str.back() == '/') { // Local hour-min-sec
            std::string hour = ts.substr(0,2);
            std::string minute = ts.substr(2,2);
            std::string second = ts.substr(4,2);

            std::ostringstream date;
            date << std::put_time(&tmLocal, "%Y-%m-%d ");
            outputTimestamp = date.str() + hour + ":" + minute + ":" + second;
        }
    } else { // If no timestamp, assume "instantaneous" and use localtime
        std::ostringstream dateTime;
        dateTime << std::put_time(&tmLocal, "%Y-%m-%d %H:%M:%S");
        outputTimestamp = dateTime.str();
    }

    return outputTimestamp;
}

// Utility Functions:
namespace BPP {

    // Match any regex!
    std::string _regexMatch(std::string _rawPacket, std::regex _pattern) {
        std::smatch matches;
        if(std::regex_search(_rawPacket, matches, _pattern)) {
            return matches[0];
        } else {
            return "None"; // Holdover from Python; descriptive enough
        }
    }
}
