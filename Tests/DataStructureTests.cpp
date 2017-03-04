/*  Link Telemetry v0.3 "Yankee Clipper"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2017-03-03

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

    DataStructureTests.cpp:

    Data structure test definitions.
*/

#include <iostream>
#include <string>

#include "TestTemplate.h"
#include "DataStructures/DecodedPacket.h"
#include "DataStructures/Packet.h"
#include "DataStructures/GroundTrack.h"

int test_DecodedPacket() {
    int result = 0;

    std::cout << "\e[1mTESTING:\e[0m DecodedPacket..." << std::endl;

    BPP::DecodedPacket packet("W3EAX-9", std::string("2015-06-13 07:17:44 EDT"), 39.657036, -77.934181, 606, 500, -1, std::string("W3EAX-9 www.umd.edu"));
	BPP::DecodedPacket pkt2;

    result += test_value(packet.callsign, std::string("W3EAX-9"), "assigned_cs");
    result += test_value(packet.timestamp, std::string("2015-06-13 07:17:44 EDT"), "assigned_ts");
    result += test_value(packet.lat, 39.657036f, "assigned_lat");
    result += test_value(packet.lon, -77.934181f, "assigned_lon");
    result += test_value(packet.alt, 606, "assigned_alt");
    result += test_value(packet.heading, 500, "assigned_heading");
    result += test_value(packet.speed, -1.0f, "assigned_speed");
    result += test_value(packet.comment, std::string("W3EAX-9 www.umd.edu"), "assigned_comment");

    result += test_value(pkt2.callsign, std::string(""), "default_cs");
    result += test_value(pkt2.timestamp, std::string(""), "default_ts");
    result += test_value(pkt2.lat, 1000.0f, "default_lat");
    result += test_value(pkt2.lon, 1000.0f, "default_lon");
    result += test_value(pkt2.alt, 0, "default_alt");
    result += test_value(pkt2.heading, 0, "default_heading");
    result += test_value(pkt2.speed, 0.0f, "default_speed");
    result += test_value(pkt2.comment, std::string(""), "default_comment");

    pkt2 = packet;
    result += test_value(pkt2.callsign, std::string("W3EAX-9"), "copied_cs");
    result += test_value(pkt2.timestamp, std::string("2015-06-13 07:17:44 EDT"), "copied_ts");
    result += test_value(pkt2.lat, 39.657036f, "copied_lat");
    result += test_value(pkt2.lon, -77.934181f, "copied_lon");
    result += test_value(pkt2.alt, 606, "copied_alt");
    result += test_value(pkt2.heading, 500, "copied_heading");
    result += test_value(pkt2.speed, -1.0f, "copied_speed");
    result += test_value(pkt2.comment, std::string("W3EAX-9 www.umd.edu"), "copied_comment");

    if(result == 0) {
        std::cout << "DecodedPacket: All tests \e[1m\e[32mPASSED.\e[0m" << std::endl;
    } else {
        std::cout << "DecodedPacket: \e[1m\e[31mFAILED\e[0m " << result << " Tests.\n";
    }

    return result;
}

int test_Packet() {

}

int test_GroundTrack() {

}
