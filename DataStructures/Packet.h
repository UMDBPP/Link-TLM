/*  Link Telemetry v0.3 "Yankee Clipper"
    
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

    Packet.h:

    Raw packet parsing and error checking class. 
    Uses DecodedPacket as its data structure.
*/

#ifndef PACK_H
#define PACK_H

#include <string>

#include "DecodedPacket.h"

namespace BPP {

class Packet {

    private:
        std::string rawPacket; // Raw packet read from RS232.
        BPP::DecodedPacket parsedPacket; // Decoded packet after parsing.
        bool validPacket; // Packet validity check variable.

        void validityCheck();

    public:
        Packet(std::string _rawPacket); // Explicit constructor
        Packet(); // Default ctor
        Packet(const Packet& oldPacket); // Storing packets on heap. Thus copy ctor.
        ~Packet(); // Dtor

        void init(std::string _rawPacket); // Init function - USE ONLY W/ DEFAULT CTOR
        void parse(); // Parse after init
        void print(); // Annnnnnd print out the packet.

        // Getters are const: do NOT modify values!
        std::string getCall() const; // Getter for callsign
        BPP::DecodedPacket getPacket() const { return parsedPacket; } // Getter for whole packet.
        bool isValid() const { return validPacket; } // Getter for validity check.

}; // Packet

} // BPP

#endif
// PACK_H
