/* Link Telemetry v0.3 "Yankee Clipper"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-04-11

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

   GroundTrack.h:

   Declares ground track container & handler class. Takes over derived quantity
   calculation from Packet itself; also does a whole lot more quantities. Also
   separates out ground tracks from different callsigns.
*/

#ifndef GNDTRACK_H
#define GNDTRACK_H

#include <string>
#include <vector>
#include <map>
#include <array>

#include "Packet.h"
#include "System/Log.h"

namespace BPP {

class GroundTrack {

	private:
		std::map<std::string, std::vector<BPP::Packet>> groundTracks; // Callsign-indexed list of recieved packets.
		BPP::Log parsedPackets; // Log file for packets we've parsed so far.

		int capturedPackets; // Number of packets we've collected and kept so far.
		bool logEnabled; // Whether we're logging captured data or not.
		float ascentRate; // ft/s
		float groundSpeed; // mph
		float downrangeDistance; // mi
		int timeToImpact; // sec
		std::array<float, 2> latlonDerivative; // Rate of change of lat and lon. lat is index 0, lon is index 1.
		std::array<std::string, 3> latestCallsigns; // Stores latest callsigns packets recieved from. Only need 2, 3 for safety. Index 0 = most recent.

		void calculateAscentRate(); // Calculate ascent rate from last couple of packets.
		void calculateGroundSpeed(); // Calculate speed from last couple of packets.
		void calculateLatLonROC(); // Calculate derivative of lat and lon.
		void calculateDownrangeDistance(); // Get downrange distance from lat and lon.
		
		float diffLatLon(BPP::Packet _firstPacket, BPP::Packet _secondPacket); // Calculate Great Circle Dist using Haversine formula.
		int diffTime(BPP::Packet _firstPacket, BPP::Packet _secondPacket); // Calculate time difference between two packets.
		std::vector<BPP::Packet> getLatest(int _numPackets = 1); // Retrieve the latest n packets from the ground track.

	public:
		GroundTrack(); // Default CTOR
		~GroundTrack(); // Another trivial DTOR!

		bool initLog(std::string _logFileName); // Logging intialization function.
		void registerCallsign(std::string _callsign); // Add a callsign to the tracking list.
		bool addPacket(std::string _rawPacket); // Parse and validity check packets; reject untracked callsigns.
		void printPacket(); // Print the latest packet and other associated data to terminal. (Convert to SI)
		
		int getNumPackets() { return capturedPackets; } // Get number of packets currently in the track.

}; // GroundTrack

} // BPP

#endif
// GNDTRACK_H
