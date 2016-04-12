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
BPP::GroundTrack::GroundTrack() : capturedPackets(0), \
	logEnabled(false), \
	ascentRate(0.0f), \
	groundSpeed(0.0f), \
	downrangeDistance(0.0f), \
	latlonDerivative { 0.0f, 0.0f } { }

// DTOR, trivial:
BPP::GroundTrack::~GroundTrack() { }

// Calculate ascent rate function.
// Simple difference right now.
// Will look into filtering very, very soon.
// (Need to make it work before I make it better.)
// Emits result in ft/s; conversion to m/s in print function.
void BPP::GroundTrack::calculateAscentRate() {
	std::vector<BPP::Packet> calcPackets = getLatest(2); // Grab latest two packets.
	int deltaAltitude = calcPackets[0].getPacket().alt - calcPackets[1].getPacket().alt; // Calc change in alt in ft.
	int deltaTime = diffTime(calcPackets[1], calcPackets[0]); // Get time between packets in sec.

	ascentRate = static_cast<float>(deltaAltitude)/static_cast<float>(deltaTime); // Calculate ascent rate in ft/s.
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

	float hours = static_cast<float>(deltaTime)/3600.0f; // Convert seconds to hours for mph units.

	groundSpeed = distanceTraveled/hours; // Finally, calculate speed in mph. Store internally.
}

// Calculates rate of change in lat and lon.
// Also not filtered for same reason as in ground speed calc.
// Units are degrees/second.
void BPP::GroundTrack::calculateLatLonROC() {

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
			index = groundTracks[latestCallsigns[1]].size() - 1; // Get the index of the second to last value.
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
			index = groundTracks[latestCallsigns[2]].size() - 2; // Get the index of the third to last value.
			packets.push_back(groundTracks[latestCallsigns[2]][index]); // Retrieve said value and add to return vector. 
		} else { // Only other case: last packet was from a call that was used for one, but not both, previous values.
			index = groundTracks[latestCallsigns[2]].size() - 1; // Since it was only accessed once before, we get second to last value.
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

}

// Add given callsign to map.
// Preempt packet reading so we can use "registered" callsigns, and ignore
// all other ones.
void BPP::GroundTrack::registerCallsign(std::string _callsign) {

}

// Parse raw, string APRS packet.
// Then validity check it.
// Discard if partial packet or untracked callsign.
// If it's good, add it to the track. Then do derived calculations.
bool BPP::GroundTrack::addPacket(std::string _rawPacket) {

}

// Print out the latest packet in a nice, formatted form.
// Includes derived data.
void printPacket() {

}
