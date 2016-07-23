/* Link Telemetry v0.3 "Yankee Clipper"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
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

   Packet.cpp:

   Definiions for Packet class.
*/

#include "Packet.h"

#include <iostream>

#include "Interface/PythonInterface.h"
#include "Parser/Parser.h"

BPP::Packet::Packet() {
    validPacket = true; // At least init this.
}

BPP::Packet::Packet(std::string _rawPacket) {
    rawPacket = _rawPacket; // Simple!
    validPacket = true;
}

// Copy constructor is essentially trivial; worth explicitly defining.
BPP::Packet::Packet(const Packet& oldPacket) {
    validPacket = oldPacket.validPacket;
    parsedPacket = oldPacket.getPacket();
    rawPacket = oldPacket.rawPacket;
}

BPP::Packet::~Packet() {} // Nothing yet.

void BPP::Packet::init(std::string _rawPacket) {
    rawPacket = _rawPacket; // init function for default CTOR
}

void BPP::Packet::validityCheck() {

    // Check if critical values are still default:
    if(parsedPacket.callsign == "" || parsedPacket.timestamp == "" || parsedPacket.lat == 1000.0f || parsedPacket.lon == 1000.0f) {
        validPacket = false;
    }

    // Check if failure values from parsing are present:
    if(parsedPacket.callsign == "None" || parsedPacket.alt == -100000) {
        validPacket = false;
    }

}

void BPP::Packet::parse() {
    BPP::Parser parser("Prefs/regex.json");

    // Get callsign
    parsedPacket.callsign = parser.getCallsign(rawPacket);

    // Get timestamp
    parsedPacket.timestamp = parser.getTimestamp(rawPacket);

    // Get Altitude
    parsedPacket.alt = parser.getAlt(rawPacket);

    // Get Course:
    parsedPacket.heading = parser.getCse(rawPacket);

    // Get Speed:
    parsedPacket.speed = parser.getSpd(rawPacket);

    // Get Comment:
    parsedPacket.comment = parser.getCmt(rawPacket);

    // Get Latitude:
    std::string uglyLat = parser.getLat(rawPacket);
    if(uglyLat == "None") {
        validPacket = false; // Validity check 
    }
    if(uglyLat.length() == 8) { // Uncompressed
        float deg = std::stof(uglyLat.substr(0,2), NULL); // Deal with the awful formatting
        float min = std::stof(uglyLat.substr(2,5), NULL);
        char dir = uglyLat[7];

        parsedPacket.lat = deg + (min/60.0f); // Convert from DMS to decimal
        if(dir == 'S') parsedPacket.lat = -parsedPacket.lat;

    } else { // Compressed
        parsedPacket.lat = BPP::PythonInterface::getFloat("BPPdecompress", "decompressLat", uglyLat); // No conversions needed here.
    }

    // Get Longitude
    std::string uglyLon = parser.getLon(rawPacket);
    if(uglyLon == "None") {
        validPacket = false; // Validity check
    }
    if(uglyLon.length() == 9) { // Uncompressed
        float deg = std::stof(uglyLon.substr(0,3), NULL); // Deal with formatting
        float min = std::stof(uglyLon.substr(3,5), NULL);
        char dir = uglyLon[8]; // Note lon is one char longer than lat.

        parsedPacket.lon = deg + (min/60.0f); // Convert DMS to decimal
        if(dir == 'W') parsedPacket.lon = -parsedPacket.lon;

    } else { // Compressed
        parsedPacket.lon = BPP::PythonInterface::getFloat("BPPdecompress", "decompressLon", uglyLon); // No conversion needed.
    }

    validityCheck();

}

void BPP::Packet::print() { // Simply print out all data from the parsed packet.

    std::cout << "Callsign: " << parsedPacket.callsign << std::endl;
    std::cout << "Time: " << parsedPacket.timestamp << std::endl;
    std::cout << "Lat: " << parsedPacket.lat << std::endl;
    std::cout << "Lon: " << parsedPacket.lon << std::endl;
    std::cout << "Alt (ft): " << parsedPacket.alt << " and (m): " << parsedPacket.alt/3.2808f << std::endl;
    std::cout << "Comment: " << parsedPacket.comment << std::endl;
}

std::string BPP::Packet::getCall() const { return parsedPacket.callsign; }