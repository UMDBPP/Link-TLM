''' Link Telemetry v0.0.1 "Gumdrop"

Copyright (c) 2015 University of Maryland Space Systems Lab
NearSpace Balloon Program

Written by Nicholas Rossomando
2015-07-11

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

regex.py:

Python script which contains regex matching functions for raw APRS packets.
Rationale: GCC on Linux Mint, which Link TM must run on for NearSpace tracking,
has a version of GCC which is four years out of date. Therefore C++ regex isn't
implemented in its library yet. The rolling-release Arch Linux user in me facepalms.

WARNING: Likely Deprecated. Kept around just in case for now.
'''

import re # Import the regex module

# Match the old tracker style of position reporting (uncompressed).
# Input is python string of the packet.
def oldPacketMatch(rawPacket):
	# Generate the Regex pattern
	oldFormat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))\\d{4}\\.\\d\\d(N|S)\\/\\d{5}\\.\\d\\d(W|E).(\\d{3}\\/\\d{3})?.*(\\/A=\\d{6})?$")
	
	# Try to match.
	# If there is no match, return false,
	# else return true.
	if oldFormat.match(rawPacket) is None:
		return False
	else:
		return True

# Match the new style of position reporting (compressed).
# Inout is python string of the packet.
def newPacketMatch(rawPacket):
	# Generate the Regex pattern
	newFormat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}( |\u00A0|[\x21-\x7A]){3}.*(\\/A=.{6})?$")

	# Try to match.
	# If no match, return false, else true.
	if newFormat.match(rawPacket) is None:
		return False
	else:
		return True

# Uncomment these and run 'python regex.py' to test.
#print newPacketMatch("W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX")
#print newPacketMatch("KB3ZZI-12>APRS,WIDE1-1,WIDE2-1:!/:h?/;%EsO   /A=000249|!8|  /KB3ZZI,125,8,27'C,http://habduino.org")
#print newPacketMatch("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000606|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev")
