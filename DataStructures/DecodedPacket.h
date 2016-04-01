/* Link Telemetry v0.1.0 "Gumdrop"
   
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

   DecodedPacket.h:

   Data structure for holding APRS packet data after extraction from the packet.
   Currently only contains a struct for position data.
*/

#ifndef DEC_PACK_H
#define DEC_PACK_H

#include <string>

namespace BPP {

// Structure containing decoded data from APRS location packets.
// Also has a constructor for easy assignment.
// Convention: Time: YYYY-MM-DD HH:MM:SS, Negative latitude is South, Negative longitude is West,
// altitude is in feet, heading is degrees clockwise from North, speed is in Knots, ascent rate is ft/s.
struct DecodedPacket {
	std::string callsign;
  std::string timestamp;
	float lat;
	float lon;
	int alt;
	int heading;
	float speed;
	std::string comment;
  float ascentRate;

	DecodedPacket(std::string cs, std::string ts, float _lat, float _lon, int _alt, int _heading, float _speed, std::string cmt) : \
		callsign(cs), \
		timestamp(ts), \
		lat(_lat), \
		lon(_lon), \
		alt(_alt), \
		heading(_heading), \
		speed(_speed), \
		comment(cmt), \
        ascentRate(0.0f) { }

   DecodedPacket() : \
        callsign(""), \
        timestamp(""), \
        lat(1000.0f), \
        lon(1000.0f), \
        alt(0), \
        heading(0), \
        speed(0.0f), \
        comment(""), \
        ascentRate(0.0f) { }
};

} // BPP

#endif
// DEC_PACK_H
