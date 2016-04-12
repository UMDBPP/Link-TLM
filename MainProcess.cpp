/* Link Telemetry v0.2.0 "Columbia"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-11-10

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

   MainProcess.cpp:

   Defines the main processes which drive the program.
*/


#include "MainProcess.h"

#include <iostream>

extern "C" {
	#include "unistd.h"
}

#include "System/Util.h"
#include "DataStructures/DecodedPacket.h"

// Initialize everything.
// Opens the setings file, opens the serial port, grabs settings.
// Also, error check the initialization of serial.
BPP::MainProcess::MainProcess() : settings("Prefs/settings.json"), initFail(false) {

	// Serial port number is hard coded in this library.
	// So you have to know the numbers in advance.
	std::cout << "Enter Serial Port Filename (Something Like /dev/ttyUSB0)\n";
	std::cin >> serialPortName;

	// Open the serial port: 9600 baud, 8 data bits, no parity, 1 stop bit.
	if(serialPort.portOpen(serialPortName, B9600, 8, 'N', 1)) {
		initFail = true;
	}

	// Set the callsigns to look for.
	// Retrieve these from JSON preferences file.
	balloonCalls = settings.getBalloonCalls();
	vanCalls = settings.getVanCalls();

	// Retrieve install location information from the JSON file as well.
	// Python needs this because it hates relative paths.
	installDirectory = settings.getInstallDirectory();

	// Open the logs.
	// Log filenames defined in same JSON file.
	allPackets.open(settings.getUnparsedLogFile());
	parsedPackets.open(settings.getParsedLogFile());

	// Print a header to the parsed log file.
	parsedPackets.log("callsign, timestamp, lat, lon, alt, heading, speed, ascent rate");

}

BPP::MainProcess::~MainProcess() { } // Thankfully, smart pointers clean up memory.

// I got to clean this up! Yay!
// This reads new packets off the serial port.
// It also attempts to reassemble packets that have been "Cut in half"
// It does this without locking up in case of a true partial packet.
void BPP::MainProcess::readSerialData() {
	int n = -1; // Indicators for recieved data lengths
	int n2 = -1;
	std::string data; // temporary data store

	// New data flag; controls if a new Packet is generated this iteration or not.
	newDataRecieved = false;

	n = serialPort.rxData(); // Pull all data from the serial port.

	// Only do this if we actually recieved data!
	if(n > 0) {
		newDataRecieved = true; // Set new data flag to true.

		data = serialPort.getData(); // Pull data into the temp string.

		if(n < 25) { // If we recieved a really short packet...
			usleep(2000); // ...wait a tiny bit (couple of milliseconds)...
			n2 = serialPort.rxData(); // ...then try again...

			if(n2 > 0) { // ...If there was more...
				data += serialPort.getData(); // ...Add it to the temp string.
			} // If there wasn't more, it was a true partial packet. These are handled by Packet()'s validity check.
		}

		if(data != lastRawPacket) { // If we didn't get a duplicate packet...
			allPackets.log(data); // ...Add it to the log!
		}

		lastRawPacket = data; // Finally, put the temp data into a class member string.
	}
}

// Parse the packets we've recieved.
// Also check packet validity.
// Much/most of this functionality will be moved to a data structure for
// holding the packets.
bool BPP::MainProcess::parseRecievedPacket() {
	bool goodPacket = false; // Initially assume a bad packet was recieved.

	// Create a new packet, place it on the stack.
	// Use C++ smart pointers to avoid memory leaks.
	std::unique_ptr<BPP::Packet> newPacket = std::make_unique<BPP::Packet>(lastRawPacket, installDirectory);
	newPacket->parse(); // Parse the packet.

	// Check to see if the packet matches any balloon callsign.
	// Also check to see if there were any parse errors.
	// If both checks are okay, we have a valid packet.
	for(size_t i=0; i<balloonCalls.size(); i++) {
		if(newPacket->getCall() == balloonCalls[i]){
			if(newPacket->isValid()) {
				goodPacket = true;
			}
		}
	}

	// Same process as above, except for the van callsigns.
	for(size_t i=0; i<vanCalls.size(); i++) {
		if(newPacket->getCall() == vanCalls[i]){
			if(newPacket->isValid()) {
				goodPacket = true;
			}
		}
	}

	// Typically, the problem is an untracked callsign.
	// More robust errors in the future would be a good idea.
	if(!goodPacket) {
		std::cout << "Untracked Callsign: " << newPacket->getCall() << std::endl;
		return false;
	}

	// Roughly calculate ascent/descent rate.
	// Not super accurate.
	// Need to filter this in the future (low pass).
	if(recievedPackets.size() > 0) {
		newPacket->calcAscentRate(*recievedPackets.back());
	}

	// If everything is good, add the packet pointer to the vector of them.
	// Also return success code.
	recievedPackets.push_back(std::move(newPacket));
	return true;
}

// Print out stuff to terminal in clean format.
void BPP::MainProcess::printLatestPacket() {
	// Access the data held in the last packet recieved.
	BPP::DecodedPacket packet = recievedPackets.back()->getPacket();
	
	// Logall of the packet data to the parsed packet log file.
	parsedPackets.log(packet.callsign, ",", \
		packet.timestamp, ",", \
		packet.lat, ",", \
		packet.lon, ",", \
		packet.alt, ",", \
		packet.heading, ",", \
		packet.speed, ",", \
		packet.ascentRate);

	// Clear the terminal screen, then print all packet information to it.
	BPP::clearTerm();
	recievedPackets.back()->print();
}

// The main loop that runs everything.
void BPP::MainProcess::mainLoop() {
	while(1) { // Go until the program is killed.
		readSerialData(); // Read incoming serial data.

		if(newDataRecieved) { // If we recieved new data,
			if(parseRecievedPacket()) { // Parse it,
				printLatestPacket(); // Then log and print it.
			}
		}

		// Delay for 1 second.
		// Needs to be non-blocking in the future.
		usleep(1000000);
	}
}