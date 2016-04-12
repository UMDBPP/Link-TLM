/* Link Telemetry v0.2.0 "Columbia"
   
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
#include <ctime>
#include <iomanip> // For std::get_time()
#include <sstream>
#include <locale>

#include "Interface/PythonInterface.h"

BPP::Packet::Packet() {
    validPacket = true; // At least init this.
}

BPP::Packet::Packet(std::string _rawPacket, std::string _installDirectory) {
    rawPacket = _rawPacket; // Simple!
    installDirectory = _installDirectory;
    validPacket = true;
}

// Copy constructor is essentially trivial; worth explicitly defining.
BPP::Packet::Packet(const Packet& oldPacket) {
    validPacket = oldPacket.validPacket;
    parsedPacket = oldPacket.getPacket();
    rawPacket = oldPacket.rawPacket;
    installDirectory = oldPacket.installDirectory;
}

BPP::Packet::~Packet() {} // Nothing yet.

void BPP::Packet::init(std::string _rawPacket, std::string _installDirectory) {
    rawPacket = _rawPacket; // init function for default CTOR
    installDirectory = _installDirectory;
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

    // Get callsign
    parsedPacket.callsign = BPP::PythonInterface::getString("BPPparser", "getCallsign", rawPacket, installDirectory);

    // Get timestamp
    parsedPacket.timestamp = BPP::PythonInterface::getString("BPPparser", "getTimestamp", rawPacket, installDirectory);

    // Get Altitude
    parsedPacket.alt = BPP::PythonInterface::getInt("BPPparser", "getAlt", rawPacket, installDirectory);

    // Get Course:
    parsedPacket.heading = BPP::PythonInterface::getInt("BPPparser", "getCse", rawPacket, installDirectory);

    // Get Speed:
    parsedPacket.speed = BPP::PythonInterface::getFloat("BPPparser", "getSpd", rawPacket, installDirectory);

    // Get Comment:
    parsedPacket.comment = BPP::PythonInterface::getString("BPPparser", "getCmt", rawPacket, installDirectory);

    // Get Latitude:
    std::string uglyLat = BPP::PythonInterface::getString("BPPparser", "getLat", rawPacket, installDirectory);
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
        parsedPacket.lat = BPP::PythonInterface::getFloat("BPPdecompress", "decompressLat", uglyLat, installDirectory); // No conversions needed here.
    }

    // Get Longitude
    std::string uglyLon = BPP::PythonInterface::getString("BPPparser", "getLon", rawPacket, installDirectory);
    if(uglyLon == "None") {
        validPacket = false; // Validity check
    }
    if(uglyLon.length() == 9) { // Uncompressed
        float deg = std::stof(uglyLon.substr(0,3), NULL); // Deal with formatting
        float min = std::stof(uglyLon.substr(3,5), NULL);
        char dir = uglyLon[8]; // Note lon is one char longer than lat.

        parsedPacket.lon = deg + (min/60.0f); // Convert DMS to decimal
        if(dir == 'S') parsedPacket.lon = -parsedPacket.lon;

    } else { // Compressed
        parsedPacket.lon = BPP::PythonInterface::getFloat("BPPdecompress", "decompressLon", uglyLon, installDirectory); // No conversion needed.
    }

    validityCheck();

}

// Calculates the average ascent rate of the balloon.
// Uses difference in timestamps and altitudes.
void BPP::Packet::calcAscentRate(const Packet& prevPacket) {
    // Get current packet timestamp in Unix time:
    // All current APRS radios use local time instead of UTC...
    std::tm tmpTime;
    std::istringstream ct(parsedPacket.timestamp);
    ct.imbue(std::locale("en_US.utf-8"));
    ct >> std::get_time(&tmpTime, "%Y-%m-%d %H:%M:%S Local");
    tmpTime.tm_isdst = 0; // Undefinied unless we define it. Since we're using differential time, DST doesn't matter!
    time_t curTime = std::mktime(&tmpTime);

    // Get the previous packet's time:
    std::istringstream pt(prevPacket.getPacket().timestamp);
    pt.imbue(std::locale("en_US.utf-8"));
    pt >> std::get_time(&tmpTime, "%Y-%m-%d %H:%M:%S Local");
    tmpTime.tm_isdst = 0;
    time_t prevTime = std::mktime(&tmpTime);

    // Get previous altitude:
    int prevAlt = prevPacket.getPacket().alt;

    // Altitude calculation:
    parsedPacket.ascentRate = (static_cast<float>(parsedPacket.alt - prevAlt))/(static_cast<float>(curTime - prevTime));
}

void BPP::Packet::print() { // Simply print out all data from the parsed packet.

    std::cout << "Callsign: " << parsedPacket.callsign << std::endl;
    std::cout << "Time: " << parsedPacket.timestamp << std::endl;
    std::cout << "Lat: " << parsedPacket.lat << std::endl;
    std::cout << "Lon: " << parsedPacket.lon << std::endl;
    std::cout << "Alt (ft): " << parsedPacket.alt << "and (m): " << parsedPacket.alt/3.2808f << std::endl;
    std::cout << "Ascent Rate (ft/s): " << parsedPacket.ascentRate << std::endl;
    std::cout << "Comment: " << parsedPacket.comment << std::endl;
}

std::string BPP::Packet::getCall() const { return parsedPacket.callsign; }