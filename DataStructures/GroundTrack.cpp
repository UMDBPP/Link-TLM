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

   GroundTrack.cpp:

   Defines ground track functions. Does heavy lifting of calculation and handling
   separate ground tracks.
*/

#include "GroundTrack.h"

#include <cmath>
#include <ctime>
#include <iomanip> // For std::get_time()
#include <sstream>
#include <locale>
#include <iostream>

#include "DecodedPacket.h"
#include "System/Util.h"

// CTOR
// Initializes values via initializer list.
// Everything else has separate function.
// Set time to impact in particular to known, obviously false default value.
BPP::GroundTrack::GroundTrack() : capturedPackets(0), \
	logEnabled(false), \
	ascentRate(0.0f), \
	groundSpeed(0.0f), \
	downrangeDistance(0.0f), \
	timeToImpact(-5), \
	latlonDerivative { 0.0f, 0.0f }, \
	latestCallsigns { "", "", "" } { }

// DTOR, trivial:
BPP::GroundTrack::~GroundTrack() { }

// Calculate ascent rate function.
// Simple difference right now.
// Will look into filtering very, very soon.
// (Need to make it work before I make it better.)
// Emits result in ft/s; conversion to m/s in print function.
// If ascent rate is negative (descent rate), also estimate time to sea level impact.
void BPP::GroundTrack::calculateAscentRate() {
	std::vector<BPP::Packet> calcPackets = getLatest(2); // Grab latest two packets.
	int deltaAltitude = calcPackets[0].getPacket().alt - calcPackets[1].getPacket().alt; // Calc change in alt in ft.
	int deltaTime = diffTime(calcPackets[1], calcPackets[0]); // Get time between packets in sec.

	if(deltaTime != 0) { // Prevent very small dt causing floating point exception/div by zero. Ascent rate will not be calculated and assumed constant between two packets so close together.
		ascentRate = static_cast<float>(deltaAltitude)/static_cast<float>(deltaTime); // Calculate ascent rate in ft/s.
	}

	if(ascentRate < 0.0f) { // If we're moving down...
		if(abs(ascentRate) > 0.5f) { // Prevent floating point exception with small ascent rates...
			timeToImpact = static_cast<int>(calcPackets[0].getPacket().alt/abs(ascentRate)); // ...Calculate an estimated time to impact.
		}
	} else { // Otherwise...
		timeToImpact = -5; // ...Reset to default value.
	}
}

// Ground speed calculation function.
// Simple difference - won't be filtered.
// Filter here would change GPS provided coords,
// And I honestly don't want to do that.
// This isn't really that critical anyway.
// Emits result in mph; conversion to m/s in print function.
void BPP::GroundTrack::calculateGroundSpeed() {
	std::vector<BPP::Packet> calcPackets = getLatest(2); // Grab two most recent packets.
	float distanceTraveled = diffLatLon(calcPackets[1], calcPackets[0]); // Get ground distance between packets.
	int deltaTime = diffTime(calcPackets[1], calcPackets[0]); // Get time between packets.

	if(deltaTime != 0) { // For floating point exception prevention
		float hours = static_cast<float>(deltaTime)/3600.0f; // Convert seconds to hours for mph units.
		groundSpeed = distanceTraveled/hours; // Finally, calculate speed in mph. Store internally.
	}
}

// Calculates rate of change in lat and lon.
// Also not filtered for same reason as in ground speed calc.
// Units are degrees/second.
void BPP::GroundTrack::calculateLatLonROC() {
	std::vector<BPP::Packet> calcPackets = getLatest(2); // Retrieve latest two packets.
	float deltaLat = calcPackets[0].getPacket().lat - calcPackets[1].getPacket().lat; // Change in latitude.
	float deltaLon = calcPackets[0].getPacket().lon - calcPackets[1].getPacket().lon; // change in longitude.
	int deltaTime = diffTime(calcPackets[1], calcPackets[0]); // Time between packets.

	if(deltaTime != 0) {
		latlonDerivative[0] = deltaLat/static_cast<float>(deltaTime); // ROC of lat in degrees/sec.
		latlonDerivative[1] = deltaLon/static_cast<float>(deltaTime); // ROC of lon in degrees/sec.
	}
}

// Get horizontal distance from launch pad.
// Assumes first packet recieved was from the pad, location-wise.
// Emits result in miles; conversion to km in print function.
void BPP::GroundTrack::calculateDownrangeDistance() {
	// Earliest packet from latest callsign. Should be fine as long as all radios are verified on the pad.
	BPP::Packet launchPad = groundTracks[latestCallsigns[0]][0];
	BPP::Packet newest = getLatest()[0]; // Only want the newest packet.

	downrangeDistance = diffLatLon(launchPad, newest); // Simply get great circle distance in miles.
}

// Get Great Circle Distance between two packets.
// Uses Haversine Formula, which is totally fine for what we're doing.
// Radius of the Earth is for approximately Maryland latitudes.
// Return value in miles. Not directly used in print, so no conversion.
// Usage convention: first argument is older packet.
float BPP::GroundTrack::diffLatLon(BPP::Packet _firstPacket, BPP::Packet _secondPacket) {
	BPP::DecodedPacket first = _firstPacket.getPacket(); // Extract packet data from given packets.
	BPP::DecodedPacket last = _secondPacket.getPacket();

	// Convert to radians!!
	first.lat = BPP::deg2rad(first.lat);
	first.lon = BPP::deg2rad(first.lon);
	last.lat = BPP::deg2rad(last.lat);
	last.lon = BPP::deg2rad(last.lon);

	float diffLat = last.lat - first.lat;
	float diffLon = last.lon - first.lon; // Get difference between the two points.

	// Haversine Equation
	float a = pow((sin(diffLat/2.0f)),2) + cos(first.lat)*cos(last.lat)*pow((sin(diffLon/2.0f)),2);
	float c = 2*atan2(sqrt(a), sqrt(1.0f-a));
	float d = 3961.0f * c; // Where 3961 is Rearth in miles.
	return d;
}

// Get difference in time between two packets.
// Returns value in seconds.
// Usage convention: first argument is older packet.
int BPP::GroundTrack::diffTime(BPP::Packet _firstPacket, BPP::Packet _secondPacket) {
	std::tm tmpTimeFirst; // Temp variables.
	std::tm tmpTimeLast;

	// Extract timestamps from given packets.
	std::istringstream firstTime(_firstPacket.getPacket().timestamp);
	std::istringstream lastTime(_secondPacket.getPacket().timestamp);

	// Set our locale:
	firstTime.imbue(std::locale("en_US.utf-8"));
	lastTime.imbue(std::locale("en_US.utf-8"));

	// Stream raw data in, parse according to given pattern.
	// Store extracted time data into temp structures.
	// Also set DST (off, but literally does not matter do to taking difference.)
	firstTime >> std::get_time(&tmpTimeFirst, "%Y-%m-%d %H:%M:%S Local");
	lastTime >> std::get_time(&tmpTimeLast, "%Y-%m-%d %H:%M:%S Local");
	tmpTimeFirst.tm_isdst = 0;
	tmpTimeLast.tm_isdst = 0;

	// After all that, we can finally convert to pure seconds.
	time_t curTime = std::mktime(&tmpTimeLast);
	time_t prevTime = std::mktime(&tmpTimeFirst);

	// Finally, subtract the time and cast to a normal int.
	return static_cast<int>(curTime-prevTime);
}

// Retrieve the latest numPackets packets from the ground track.
// Defaults to 1.
// Handles cases where these packets have come from the same or different callsigns.
// Returns a vector of the packets.
// Return convention: index 0 is the newest packet, 1 is second newest, etc.
// Throws error if more than 3 packets requested; returns latest 3 in this case.
std::vector<BPP::Packet> BPP::GroundTrack::getLatest(int _numPackets) {
	std::vector<BPP::Packet> packets; // Return value

	// First, check input:
	if(_numPackets < 1) {
		std::cerr << "Too few packets requested from BPP::GroundTrack::getLatest.\n";
		std::cerr << "Returning one packet (latest)\n";
		_numPackets = 1;
	} else if(_numPackets > 3) {
		std::cerr << "Too many packets requested from BPP::GroundTrack::getLatest.\n";
		std::cerr << "Returning three latest packets.\n";
		_numPackets = 3;
	}

	// Adding the first value will always be the same: the end of the callsign vector most recently added to.
	packets.push_back(groundTracks[latestCallsigns[0]].back());
	
	// Now, handle second requested value (if requested):
	size_t index; // For accessing vectors by index instead of back()
	if(_numPackets > 1) {
		if(latestCallsigns[0] != latestCallsigns[1]) { // If newest two came from different callsigns, pretty easy.
			packets.push_back(groundTracks[latestCallsigns[1]].back()); // Because we can use back() on a different vector.
		} else { // Otherwise, have to handle not using back():
			index = groundTracks[latestCallsigns[1]].size() - 2; // Get the index of the second to last value.
			packets.push_back(groundTracks[latestCallsigns[1]][index]); // Retrieve said value and add to return vector. 
		}
	}

	// Finally, and most complex, handle a possible third value:
	if(_numPackets > 2) {
		// If we got lucky and don't have duplicates at all:
		if((latestCallsigns[2] != latestCallsigns[1]) && (latestCallsigns[2] != latestCallsigns[0])) {
			packets.push_back(groundTracks[latestCallsigns[2]].back()); // It's the lovely back() trick.
		} else if((latestCallsigns[2] == latestCallsigns[1]) && (latestCallsigns[2] == latestCallsigns[0])) {
			// If all three latest packets were from the same call:
			index = groundTracks[latestCallsigns[2]].size() - 3; // Get the index of the third to last value.
			packets.push_back(groundTracks[latestCallsigns[2]][index]); // Retrieve said value and add to return vector. 
		} else { // Only other case: last packet was from a call that was used for one, but not both, previous values.
			index = groundTracks[latestCallsigns[2]].size() - 2; // Since it was only accessed once before, we get second to last value.
			packets.push_back(groundTracks[latestCallsigns[2]][index]);
		}
	}

	// And finally, return:
	return packets;
}

// Initialize logging.
// Open provided log file.
// If that doesn't happen for whatever reason, contimue without logging.
// Also inform user that that is the case.
bool BPP::GroundTrack::initLog(std::string _logFileName) {
	logEnabled = parsedPackets.open(_logFileName); // Open the log file, report if success.

	if(!logEnabled) { // If log didn't open, inform user, but don't crash.
		std::cerr << "Logging of parsed packets not enabled: " << _logFileName << " not opened!\n";
		std::cerr << "Proceeding without parsed packet logging...\n";
	} else { // If it did, print data header:
		parsedPackets.log("Callsign,Timestamp,Lat,Lon,Alt(ft),Downrange Distance(mi),Ascent Rate(ft/s),Ground Speed(mph)");
	}

	return logEnabled; // Return log status (In case user wants this to be a failure case...)
}

// Add given callsign to map.
// Preempt packet reading so we can use "registered" callsigns, and ignore
// all other ones.
void BPP::GroundTrack::registerCallsign(std::string _callsign) {
	std::vector<BPP::Packet> tempVect; // Create empty vector to add to the map.

	groundTracks[_callsign] = tempVect; // Insert vector into map with callsign as key.

	// Feedback for user: should know what callsigns they're tracking!
	std::cout << "Added Callsign " << _callsign << " to list of tracked callsigns.\n";
}

// Parse raw, string APRS packet.
// Then validity check it.
// Discard if partial packet or untracked callsign.
// If it's good, add it to the track. Then do derived calculations.
bool BPP::GroundTrack::addPacket(std::string _rawPacket) {
	 // Begin assuming we've recieved an untracked packet.
	bool trackedPacket = false;

	BPP::Packet tempPacket(_rawPacket); // Make temporary packet for recieved data.
	tempPacket.parse(); // Parse the raw data.

	// Now for checking. First, we check for callsign.
	// Untracked callsigns are by far the most common "invalid" packet type.
	// Also, range-based for loops are great.
	for(auto const& track : groundTracks) {
		if(tempPacket.getCall() == track.first) { // If we match a callsign...
			trackedPacket = true; // ...we have a packet we want to track.
		}
	}

	// At this point, if we don't have a tracked packet, we can stop:
	if(!trackedPacket) {
		std::cout << "Untracked Callsign: " << tempPacket.getCall() << std::endl; // Provide user feedback.
		return false;
	}

	// Then, we check for duplicate packets:
	if(latestCallsigns[0] != "") {
		BPP::Packet latest = getLatest()[0];
		BPP::DecodedPacket oldPacket = latest.getPacket();
		BPP::DecodedPacket newPacket = tempPacket.getPacket();

		if((oldPacket.alt == newPacket.alt) && (oldPacket.callsign == newPacket.callsign)) { // IF both these values are exactly the same, it's a duplicate.
			std::cout << "Duplicate Packet. Not added to track.\n"; // Report...
			return false; // ...and return.
		}
	}

	// Now we check if we recieved a valid (not partial) packet.
	// This doesn't care about data in the comment string, only actual tracking data.
	// So comments CAN still be dropped.
	if(!tempPacket.isValid()) { // If the packet isn't valid due to data loss...
		std::cout << "Partial Packet. Not added to track.\n"; // ...Provide feedback.
		return false; // And stop.
	}

	// Now, if we've gotten down here, all is good, and we can safely add the packet to the track.
	groundTracks[tempPacket.getCall()].push_back(tempPacket); // Add the new packet to the end of its callsign vector.

	// Also, shift the array of latest callsigns!
	latestCallsigns[2] = latestCallsigns[1];
	latestCallsigns[1] = latestCallsigns[0];
	latestCallsigns[0] = tempPacket.getCall();

	// Finally, calculate derived values:
	if(latestCallsigns[1] != "") { // Make sure we have more than one packet to work with
		calculateAscentRate();
		calculateGroundSpeed();
		calculateLatLonROC();
		calculateDownrangeDistance();
	}

	return true; // And report success!
}

// Print out the latest packet in a nice, formatted form.
// Also, if logging is enabled, add it to the log!
// Includes derived data.
void BPP::GroundTrack::printPacket() {
	BPP::Packet latestPacket = getLatest()[0]; // Get the latest packet, first of all.

	latestPacket.print(); // First use packet's built-in print.
	// Now print derived quantities:
	std::cout << "Downrange Distance (mi): " << downrangeDistance << " and (km): " << downrangeDistance/0.62137f << std::endl;
	std::cout << "Ascent Rate (ft/s): " << ascentRate << " and (m/s): " << ascentRate*0.3048f << std::endl;
	std::cout << "Ground Speed (mph): " << groundSpeed << " and (m/s): " << groundSpeed*0.44704f << std::endl;
	std::cout << "Lat Rate of Change (degrees/s): " << latlonDerivative[0] << std::endl;
	std::cout << "Lon Rate of Change (degrees/s): " << latlonDerivative[1] << std::endl;
	if(timeToImpact != -5) { // If we have an estimated impact time, show it.
		std::cout << "Estimated time to (sea level) impact (sec): " << timeToImpact << std::endl;
	}
	std::cout << std::endl; // Throw in an extra newline to help separate data.

	// Now, log what we need to, if logging is on:
	if(logEnabled) {
		BPP::DecodedPacket packetData = latestPacket.getPacket(); // Extract the data from the packet.

		// And log everthing!
		parsedPackets.log(packetData.callsign, ",", \
			packetData.timestamp, ",", \
			packetData.lat, ",", \
			packetData.lon, ",", \
			packetData.alt, ",", \
			downrangeDistance, ",", \
			ascentRate, ",", \
			groundSpeed);
	}
}
