#include <iostream>
#include <string>
#include <vector>

#include "DataStructures/DecodedPacket.h"

template<typename T> int test_value(T _value, T _correctVal, std::string _testName = "GENERIC TEST") {
    if(_value == _correctVal) {
        return 0;
    }

    std::cerr << "\e[1m\e[31mFAILED TEST: \e[0m" << _testName << std::endl;
    return 1;
}

int test_DecodedPacket();
int test_Packet();

int main(void) {
    test_DecodedPacket();

    return 0;
}

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
