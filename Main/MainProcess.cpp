/* Link Telemetry v1.0 "Odyssey"
   
   Copyright (c) 2015-2017 University of Maryland Space Systems Lab
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
#include <thread>

extern "C" {
	#include "unistd.h"
}

#include "System/Util.h"
#include "DataStructures/DecodedPacket.h"

// Initialize everything.
// Gets settings, initializes ground track, opens logs, starts Python interpreter.
// Also, print ASCII art splash screen.
BPP::MainProcess::MainProcess() : settings("Prefs/settings.json"), initFail(false) {

	// Print Program Banner at start of program:
	BPP::LinkTlm();

	// Set the callsigns to look for.
	// Retrieve these from JSON preferences file.
	// For balloon callsigns, "register" with ground track.
	// If no callsigns read fron file, default to one.
	balloonCalls = settings.getStringVector("balloonCallsigns");
	if(balloonCalls.size() == 0) {
		std::cerr << "WARNING: No balloon callsigns specified. Using default W3EAX-9.\n";
		balloonCalls.push_back("W3EAX-9");
	}

	for(const std::string cs : balloonCalls) {
		trackedPackets.registerCallsign(cs);
	}

	vanCalls = settings.getStringVector("vanCallsigns");
	if(vanCalls.size() == 0) {
		std::cerr << "WARNING: No van callsigns specified. Van tracking is disabled!\n";
	}

	// Open the logs.
	// Check for/create Logs directory
	BPP::createDir("Logs");

	// Log filenames defined in same JSON file.
	std::string logFile = "";
	logFile = settings.getString("unparsedLogFilename");
	if(logFile == "") {
		logFile = "Logs/unparsedPackets.txt";
		std::cerr << "Unparsed packet log file not specified! Using Logs/unparsedPackets.txt.\n";
	}
	allPackets.open(logFile);
	
	logFile = "";
	logFile = settings.getString("parsedLogFilename");
	if(logFile == "") {
		logFile = "Logs/parsedPackets.txt";
		std::cerr << "Parsed packet log file not specified! Using Logs/parsedPackets.txt.\n";
	}
	trackedPackets.initLog(logFile); // Parsed log in GroundTrack.

	logFile = "";
	logFile = settings.getString("jsonLogFilename");
	if(logFile == "") {
		logFile = "Logs/jsonOutput.json";
		std::cerr << "JSON file name not specified! Using Logs/jsonOutput.json.\n";
	}
	trackedPackets.jsonEnable(logFile);

	logFile = "";
	logFile = settings.getString("kmlLogFilename");
	if(logFile == "") {
		logFile = "Logs/kmlOutput.kml";
		std::cerr << "KML file name not specified! Using Logs/kmlOutput.kml.\n";
	}
	trackedPackets.kmlEnable(logFile);

	// Set our exit code to false to start:
	exitCode = false;

}

BPP::MainProcess::~MainProcess() { }

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
// Most former functionality moved to GroundTrack class.
// Keep this for program flow clarity, though.
// Rewrite dropped payload functionality after NS-60.
bool BPP::MainProcess::parseRecievedPacket() {
	return trackedPackets.addPacket(lastRawPacket);
}

// Print out stuff to terminal in clean format.
void BPP::MainProcess::printLatestPacket() {
	// Clear the terminal screen, then print all packet information to it.
	// Also logs to file.
	BPP::clearTerm();
	trackedPackets.printPacket();
}

// Should be called from a different thread to not block main loop.
// Looks for program exit code (q or Q) on stdin.
// Could also be used for other input in future.
void BPP::MainProcess::readUserInput() {
	char code;
	
	while(!exitCode) { // For the life of the program...
		std::cin >> code; // read a single character from stdin.

		if((code == 'q') || (code == 'Q')) { // If user sent quit code.
			exitCode = true; // Set atomic (thread-safe) bool to true.
		}
	}
}

// Allow user to specify serial port filename via command line argument.
// Requests from stdin if not (Default parameter is empty string).
void BPP::MainProcess::initSerial(std::string argv) {
	std::string serialPortName; // Temp variable

	if(argv == "") { // If cmd line arg not supplied, ask user.
		std::cout << "Enter Serial Port Filename (Something Like /dev/ttyUSB0)\n";
		std::cin >> serialPortName;
	} else {
		serialPortName = argv; // otherwise, use cmd line arg.
	}

	// Open the serial port: 9600 baud, 8 data bits, no parity, 1 stop bit.
	if(serialPort.portOpen(serialPortName, B9600, 8, 'N', 1)) {
		initFail = true; // Failure check.
	}
}

// The main loop that runs everything.
void BPP::MainProcess::mainLoop() {
	std::thread inputThread(&BPP::MainProcess::readUserInput, this); // Spawn input thread

	while(!exitCode) { // Go until the program is killed.
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

	inputThread.join(); // Kill input thread after loop exit.
}