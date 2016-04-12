#include "GroundTrack.h"

#include <cmath>

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
	latlonDerivative[0](0.0f), \
	latlonDerivative[1](0.0f) { }

// DTOR, trivial:
BPP::GroundTrack::~GroundTrack() { }

// Calculate ascent rate function.
// Simple difference right now.
// Will look into filtering very, very soon.
// (Need to make it work before I make it better.)
// Emits result in ft/s; conversion to m/s in print function.
void BPP::GroundTrack::calculateAscentSpeed() {

}

// Ground speed calculation function.
// Simple difference - won't be filtered.
// Filter here would change GPS provided coords,
// And I honestly don't want to do that.
// This isn't really that critical anyway.
// Emits result in mph; conversion to m/s in print function.
void BPP::GroundTrack::calculateGroundSpeed() {

}

// Calculates rate of change in lat and lon.
// Also not filtered for same reason as in ground speed calc.
// Units are degrees/second.
void BPP::GroundTrack::calculateLatLonROC() {

}

// Get horizontal distance from launch pad.
// Emits result in miles; conversion to km in print function.
void BPP::GroundTrack::calculateDownrangeDistance() {

}

// Get Great Circle Distance between two packets.
// Uses Haversine Formula, which is totally fine for what we're doing.
// Radius of the Earth is for approximately Maryland latitudes.
// Return value in miles. Not directly used in print, so no conversion.
float BPP::GroundTrack::diffLatLon(BPP::Packet _firstPacket, BPP::Packet _secondPacket) {
	BPP::DecodedPacket first = _firstPacket.getPacket(); // Extract packet data from given packets.
	BPP::DecodedPacket last = _secondPacket.getPacket();

	float diffLat = second.lat - first.lat;
	float diffLon = second.lon - first.lon; // Get difference between the two points.

	// Haversine Equation
	float a = pow((sin(diffLat/2.0f)),2) + cos(first.lat)*cos(last.lat)*pow((sin(diffLon/2.0f)),2);
	float c = 2*atan2(sqrt(a), sqrt(1.0f-a));
	float d = 3961.0f * c; // Where 3961 is Rearth in miles.
	return d;
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
