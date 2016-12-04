/* Link Telemetry v0.3 "Yankee Clipper"
   
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

   MainProcess.h:

   Declares the class which handles main program execution.
*/

#ifndef MAINPROC_H
#define MAINPROC_H

#include <vector>
#include <string>
#include <atomic>

#include "DataStructures/Packet.h"
#include "DataStructures/GroundTrack.h"
#include "System/JSONLoader.h"
#include "System/Log.h"
#include "Interface/RS232.h"

namespace BPP {

class MainProcess {

	private:
		BPP::GroundTrack trackedPackets; // Keeps tabs on all recieved packets and calculates derived quantities.
		BPP::JSONLoader settings; // Loads JSON settings file - filename in initialzer list.
		BPP::Log allPackets; // Log file for all recieved, unparsed packets.
		BPP::RS232Serial serialPort; // Object for managing the serial port.

		// Possibly move these into contructor as local variables. Not used elsewhere. After NS-60.
		std::vector<std::string> balloonCalls; // Balloon callsigns we're flying (as defined in settings.)
		std::vector<std::string> vanCalls; // Callsigns of vans being tracked, if any. (Also defined in settings.)

		bool initFail; // Initialization failure checking.
		bool newDataRecieved; // Check for if new data has come in.
		std::atomic<bool> exitCode; // Check if exit interrupt recieved.
		std::string serialPortName; // Filename of serial port. (For the dev team: in Unix, everything is a file!)
		std::string lastRawPacket; // Latest valid raw APRS packet recieved.

		// Read data from the serial port.
		void readSerialData();

		// Parse serial data into a packet.
		// True return means we recieved a good packet.
		bool parseRecievedPacket();

		// Print packet to terminal.
		void printLatestPacket();

		// Scan user keyboard input for exit code.
		void readUserInput();

	public:
		// Default Constructor
		MainProcess();
		~MainProcess(); // DTOR (though smart pointers keep this trivial)

		void initSerial(std::string _argv = ""); // Allow command line arg to init serial.

		bool failed() { return initFail; } // Getter for initialization failure status.

		// Program main logic loop.
		void mainLoop();

}; // MainProcess

} // BPP

#endif
// MAINPROC_H