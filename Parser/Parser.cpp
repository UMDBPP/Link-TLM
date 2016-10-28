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

#include <ctime>
#include <iomanip>
#include <sstream>
#include <cmath>
    
#include "System/JSONLoader.h"

// Match any regex!
std::string BPP::Parser::regexMatch(std::string _rawPacket, std::regex _pattern) {
    std::smatch matches;
    if(std::regex_search(_rawPacket, matches, _pattern)) {
        return matches[0];
    } else {
        return "None"; // Holdover from Python; descriptive enough
    }
}

BPP::Parser::Parser(std::string _regexFilename) {
    JSONLoader regexFile(_regexFilename); // Load the Json file

    // Store all the regex pattern strings in the internal struct.
    cs.assign(regexFile.getString("callsignPattern"));
    ts.assign(regexFile.getString("timestampPattern"));
    compLatLon.assign(regexFile.getString("compressedLatLonPattern"));
    uncompLat.assign(regexFile.getString("uncompressedLatPattern"));
    uncompLon.assign(regexFile.getString("uncompressedLonPattern"));
    alt.assign(regexFile.getString("altitudePattern"));
    compCseSpd.assign(regexFile.getString("compressedCseSpdPattern"));
    uncompCseSpd.assign(regexFile.getString("uncompressedCseSpdPattern"));
    uncompCmt.assign(regexFile.getString("uncompressedCommentPattern"));
    compCmt.assign(regexFile.getString("compressedCommentPattern"));
}

BPP::Parser::~Parser() { }

std::string BPP::Parser::getCallsign(std::string _rawPacket) {
    return regexMatch(_rawPacket, cs); // Simply search for and return the callsign
}

std::string BPP::Parser::getTimestamp(std::string _rawPacket) {
    std::string matchStr = regexMatch(_rawPacket, ts);
    std::string outputTimestamp = "None";

    // Get system time to fill missing details
    auto t = std::time(nullptr);
    auto tmLocal = *std::localtime(&t);
    auto tmUTC = *std::gmtime(&t);

    if(matchStr != std::string("None")) { // If timestamp was explicit in packet, extract it
        std::string tsStr = _rawPacket.substr(matchStr.size(),7);
        if(tsStr.back() == 'h') { // This is the UTC hour-min-sec format
            std::string hour = tsStr.substr(0,2);
            std::string minute = tsStr.substr(2,2);
            std::string second = tsStr.substr(4,2);

            std::ostringstream date;
            date << std::put_time(&tmUTC, "%Y-%m-%d "); // Extract just the date
            outputTimestamp = date.str() + hour + ":" + minute + ":" + second + " UTC";
        } else if(tsStr.back() == 'z') { // UTC day-hour-minute format
            std::string day = tsStr.substr(0,2);
            std::string hour = tsStr.substr(2,2);
            std::string minute = tsStr.substr(4,2);

            std::ostringstream ym;
            std::ostringstream second;
            ym << std::put_time(&tmUTC, "%Y-%m-");
            second << std::put_time(&tmUTC, "%S");
            outputTimestamp = ym.str() + day + " " + hour + ":" + minute + ":" + second.str() + " UTC";
        } else if(tsStr.back() == '/') { // Local hour-min-sec
            std::string hour = tsStr.substr(0,2);
            std::string minute = tsStr.substr(2,2);
            std::string second = tsStr.substr(4,2);

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

std::string BPP::Parser::getLat(std::string _rawPacket) {
    std::string unCompPacket = regexMatch(_rawPacket, uncompLat); // Check for both commpressed and uncompressed matches
    std::string compPacket = regexMatch(_rawPacket, compLatLon);

    // Check for uncompressed first and return if found.
    if(unCompPacket != std::string("None")) {
        return unCompPacket;
    }
    // Then check for compressed
    if(compPacket != std::string("None")) {
        return compPacket.substr(compPacket.size()-9,4); // Return only the Lat bits.
    }

    return "None"; // If we got here, Lat was not found (Invalid packet).
}

std::string BPP::Parser::getLon(std::string _rawPacket) {
    std::string unCompPacket = regexMatch(_rawPacket, uncompLon);
    std::string compPacket = regexMatch(_rawPacket, compLatLon);

    if(unCompPacket != std::string("None")) {
        return unCompPacket;
    }

    if(compPacket != std::string("None")) {
        return compPacket.substr(compPacket.size()-5,4);
    }

    return "None";
}

int BPP::Parser::getAlt(std::string _rawPacket) {
    std::string strAlt = regexMatch(_rawPacket, alt);
    if(strAlt != std::string("None")) {
        return std::stoi(strAlt.substr(3)); // Convert altitude from string to int if it exists.
    }

    return -100000; // If it doesn't exist, return expected error value (-100000 feet is impossible!)
}

int BPP::Parser::getCse(std::string _rawPacket) {
    std::string compStrCse = regexMatch(_rawPacket, compCseSpd);
    std::string uncompStrCse = regexMatch(_rawPacket, uncompCseSpd);

    if(uncompStrCse != std::string("None")) {
        return std::stoi(uncompStrCse.substr(2,3)); // If uncomp, extract and return.
    }
    if(compStrCse != std::string("None")) {
        int checkByte = (int)compStrCse.at(compStrCse.size()-1); // Grab the last character of the field.
        if((checkByte & 8) != 0) { // Two bitwise checks tell whether the field exists.
            if((checkByte & 16) != 0) {
                int cseByte = (int)compStrCse.at(compStrCse.size()-3);
                return (cseByte-33)*4; // Wacky decompression algo.
            }
        }
    }

    return -1; // It's totally possible for this field to not exist; -1 is expected failure code.
}

float BPP::Parser::getSpd(std::string _rawPacket) {
    std::string compStrSpd = regexMatch(_rawPacket, compCseSpd);
    std::string uncompStrSpd = regexMatch(_rawPacket, uncompCseSpd);

    if(uncompStrSpd != std::string("None")) {
        return std::stoi(uncompStrSpd.substr(6)); // If uncompressed, extract and return
    }
    if(compStrSpd != std::string("None")) {
        int checkByte = (int)compStrSpd.at(compStrSpd.size()-1);
        if((checkByte & 8) != 0) {
            if((checkByte & 16) != 0) {
                int spdByte = (int)compStrSpd.at(compStrSpd.size()-2);
                float exponent = spdByte-33; // Weird decompression algorithm 2
                float knots = pow(1.08f, exponent);
                return knots*1.15f; // Conversion to mph as well.
            }
        }
    }

    return -1.0; // Again, not necessary for this to exist, so return an expected value if it doesn't.
}

std::string BPP::Parser::getCmt(std::string _rawPacket) {
    std::string compCmtStr = regexMatch(_rawPacket, compCmt);
    std::string uncompCmtStr = regexMatch(_rawPacket, uncompCmt);

    if(uncompCmtStr != "None") {
        return std::regex_replace(_rawPacket, uncompCmt, ""); // Strip the main body of the packet, leaving only the comment
    }

    if(compCmtStr != "None") {
        return std::regex_replace(_rawPacket, compCmt, "");
    }

    return "None"; // This should never, or rarely, happen.
}

float BPP::Parser::decompressLat(std::string _compLat) {
    float x1 = _compLat[0] - 33.0f; // Get int values of each character
    float x2 = _compLat[1] - 33.0f; // Then subtract 33 and go to float
    float x3 = _compLat[2] - 33.0f;
    float x4 = _compLat[3] - 33.0f;

    // Finally, decompress according to APRS spec.
    float lat = 90.0 - (x1*pow(91,3) + x2*pow(91,2) + x3*91 + x4)/380926.0f;
    return lat;
}

float BPP::Parser::decompressLon(std::string _compLon) {
    float x1 = _compLon[0] - 33.0f; // Get int values of each character
    float x2 = _compLon[1] - 33.0f; // Then subtract 33 and go to float
    float x3 = _compLon[2] - 33.0f;
    float x4 = _compLon[3] - 33.0f;

    // Finally, decompress according to APRS spec.
    float lon = -180.0f + (x1*pow(91,3) + x2*pow(91,2) + x3*91 + x4)/190463.0f;
    return lon;
}
