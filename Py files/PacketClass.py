
"""
/* Link Telemetry v2.0 "Mobile"

   Copyright (c) 2015-2017 University of Maryland Space Systems Lab
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

   PacketClass.py:

   Raw packet parsing and error checking class.
    Uses DecodedPacket as its data structure.
*/
"""
import DecodedPacketClass


class Packet(DecodedPacketClass.DecodedPacket):

    def __init__(self, oldPacket):
        DecodedPacketClass.DecodedPacket.__init__(cs,ts,_lat,_lon,_alt,_heading,_speed,cmt)
        self.rawPacket = oldPacket.rawPacket  # Raw packet read from RS232.
        self.parsedPacket = oldPacket.parsedPacket
        self.validPacket = True


    def validityCheck(self):
        pass

    def parse(self):
        pass

    def prnt(self):
        pass

    def getCall(self):
        pass

    def isValid(self):
        pass

    def getPacket(self):
