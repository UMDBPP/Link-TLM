/*  Link Telemetry v1.0 "Odyssey"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
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

    Parser.h:

    Declarations for C++ based parsing to replace Python parsing.
    Implemented as functions behind a namespace, like PythonInterface (which
    will be deprecated in time.)

    Parser loads regex strings from a JSON file, so they're still easily editable.
*/

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <regex>

namespace BPP {

class Parser {

    private:
        std::regex cs; // Callsign
        std::regex ts; // Timestamp
        std::regex compLatLon;
        std::regex uncompLat;
        std::regex uncompLon;
        std::regex alt; // Altitude
        std::regex compCseSpd; // Course/speed
        std::regex uncompCseSpd;
        std::regex uncompCmt; // Comment
        std::regex compCmt;

        std::string regexMatch(std::string _rawPacket, std::regex _pattern);

    public:
        Parser(std::string _regexFilename); // Load all the regexes from file and store them internally.
        ~Parser();

        // Parsing functions. These a direct, drop in replacements for the Python:

        // Extract callsign from raw data, return as a string.
        std::string getCallsign(std::string _rawPacket);

        // Extract timestamp if it exists; create one for "right now" if it doesn't.
        // Assumption: we recieved the packet today.
        // Returns time in string format.
        std::string getTimestamp(std::string _rawPacket);

        // Extract Latitude in ddmm.mm or compressed format.
        // Return as string; conversion code in Packet structure already.
        std::string getLat(std::string _rawPacket);

        // Extract Longitude in dddmm.mm or compressed format.
        // Return as string for same reason as above.
        std::string getLon(std::string _rawPacket);

        // Extract altitude in feet from the packet
        // Return as an int; conversion to meters is done when packets are printed.
        int getAlt(std::string _rawPacket);

        // Extract heading, if present, from the APRS data
        // Returns degrees from North as int (if the course field exists)
        int getCse(std::string _rawPacket);

        // Extract speed, if present, from the APRS data
        // Returns speed in mph as float if it exists
        float getSpd(std::string _rawPacket);

        // Extract the APRS comment field.
        // In our packets, this will include the altitude data.
        std::string getCmt(std::string _rawPacket);

        // Latitude and Longitude decompression functions
        float decompressLat(std::string _compLat);
        float decompressLon(std::string _compLon);
}; // Parser

} // BPP

#endif // PARSER_H
