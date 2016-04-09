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

   MainProcess.h:

   Declares the class which handles main program execution.
*/

#ifndef MAINPROC_H
#define MAINPROC_H

#include <vector>
#include <memory>
#include <string>

#include "DataStructures/Packet.h"
#include "System/JSONLoader.h"
#include "System/Log.h"

namespace BPP {

class MainProcess {

	private:
		// Vector of all recieved packets.
		std::vector<std::unique_ptr<BPP::Packet>> recievedPackets; // On the heap, using smart pointers.
		BPP::JSONLoader settings; // Loads JSON settings file - filename in initialzer list.
		BPP::Log allPackets; // Log file for all recieved, unparsed packets.
		BPP::Log parsedPackets;// Log file for all parsed packets from a recognized callsign.

		std::string installDirectory; // Install directory specified in settings.

		std::vector<std::string> balloonCalls; // Balloon callsigns we're flying (as defined in settings.)
		std::vector<std::string> vanCalls; // Callsigns of vans being tracked, if any. (Also defined in settings.)

		bool initFail; // Initialization failure checking.
		bool newDataRecieved; // Check for if new data has come in.
		int serialPort; // Serial port number, as defined by rs232 lib.
		std::string lastRawPacket; // Latest valid raw APRS packet recieved.

		// Read data from the serial port.
		void readSerialData();

		// Parse serial data into a packet.
		// True return means we recieved a good packet.
		bool parseRecievedPacket();

		// Print packet to terminal.
		void printLatestPacket();

	public:
		// Default Constructor
		MainProcess();
		~MainProcess(); // DTOR (though smart pointers keep this trivial)

		bool failed() { return initFail; } // Getter for initialization failure status.

		// Program main logic loop.
		void mainLoop();

}; // MainProcess

} // BPP

#endif
// MAINPROC_H