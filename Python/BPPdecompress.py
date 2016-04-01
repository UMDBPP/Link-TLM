''' Link Telemetry v0.1.0 "Charlie Brown"

Copyright (c) 2015-2016 University of Maryland Space Systems Lab
NearSpace Balloon Program

Written by Nicholas Rossomando
2015-07-11
2015-10-30 : Comments

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

BPPdecompress.py:

Python script which contains functions to decompress compressed APRS packets.
Rationale: Python is way better for parsing than C++. Seriously.
'''

import math

def decompressLat(compData):
	y1 = ord(compData[0]) - 33.0 # Get the integer values of each
	y2 = ord(compData[1]) - 33.0 # ASCII character in the string.
	y3 = ord(compData[2]) - 33.0 # Then subtract 33.
	y4 = ord(compData[3]) - 33.0 # Yes, this is dumb.

	# Now use latitude formula providedin APRS spec
	lat = 90.0 - (y1*pow(91,3) + y2*pow(91,2) + y3*91 + y4)/380926.0
	return lat

def decompressLon(compData):
	x1 = ord(compData[0]) - 33.0
	x2 = ord(compData[1]) - 33.0
	x3 = ord(compData[2]) - 33.0
	x4 = ord(compData[3]) - 33.0

	# Use longitude formula providedin APRS spec (which is different from Lat!)
	lon = -180.0 + (x1*pow(91,3) + x2*pow(91,2) + x3*91 + x4)/190463.0
	return lon

#print decompressLon("<*e7")