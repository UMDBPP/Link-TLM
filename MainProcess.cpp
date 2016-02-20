/* Link Telemetry v0.1.0 "Charlie Brown"
   
   Copyright (c) 2015 University of Maryland Space Systems Lab
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
//#include "rs232.h"

#include "System/Util.h"
#include "DataStructures/DecodedPacket.h"

// Initialize everything.
// Opens the setings file, opens the serial port, grabs settings.
// Also, error check the initialization of serial.
BPP::MainProcess::MainProcess() : settings("Prefs/settings.json"), initFail(false) {

	// Serial port number is hard coded in this library.
	// So you have to know the numbers in advance.
	std::cout << "Enter Serial Port Number (As defined by lib docs.)\n";
	std::cin >> serialPort;

	// Open the serial port, 8 data bits, no parity, 1 stop bit.
	//if(RS232_OpenComport(serialPort, 9600, std::string("8N1").c_str())) {
	//	initFail = true;
	//}

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

// This is a horrid, awful mess.
// Let's maybe fix this when we replace the serial lib?
void BPP::MainProcess::readSerialData() {
	int n = -1;
	int n2 = -1;
	unsigned char buf[4096];
	std::string data;

	newDataRecieved = false;

	//n = RS232_PollComport(serialPort, buf, 4095);

	if(n > 0) {
		newDataRecieved = true;

		buf[n] = 0;
		data = (char *)buf;

		if(n < 25) {
			usleep(2000);
			//n2 = RS232_PollComport(serialPort, buf, 4095);

			if(n2 > 0) {
				buf[n] = 0;
				data += (char *)buf;
			}
		}

		if(data != lastRawPacket) {
			allPackets.log(data);
		}

		lastRawPacket = data;
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
		// Needs t obe non-blocking in the future.
		usleep(1000000);
	}
}