''' Link Telemetry v0.1.0 "Charlie Brown"

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

BPPparser.py:

Python script which contains functions to parse raw APRS packets.
Rationale: Python is way better for parsing than C++. Seriously.
'''

import re # Import the regex module
import datetime # Import the timestamp module
import math # For pow

# Extract the callsign the packet was sent from.
# Takes raw packet as python string.
# Returns callsign as python string.
def getCallsign(rawPacket):
	csPattern = re.compile("[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?") # Define regex pattern for cs, with or without SSID
	cs = csPattern.search(rawPacket) # Find the first matching cs (which IS the source)
	if cs is not None:
		return cs.group(0) # Return the callsign.

	return "None" # If null, return empty string for safety.

# Extract a timestamp if present.
# Takes raw packet as Python string.
# Returns string-formatted timestamp.
# Assumes packets came in...you know...today.
def getTimestamp(rawPacket):
	tsExistsPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d){1,2}?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:(\\/|@)") # Regex to see if ts is in packet
	ts = tsExistsPat.search(rawPacket) # Check if ts is in packet.

	if ts is not None: # If ts is in packet, build ts string from it.
		time = rawPacket[ts.end():(ts.end()+7)] # Extract the actual time info

		# If time is in hour-min-sec UTC format:
		if time[-1] is 'h':
			# Extract the h/m/s:
			hour = time[0:2]
			minute = time[2:4]
			second = time[4:6]

			# Get the date from the system:
			date = datetime.datetime.utcnow().strftime("%Y-%m-%d")

			# Concat date and time:
			timestamp = date + " " + hour + ":" + minute + ":" + second

		# If time is in the day-hour-min zulu format:
		elif time[-1] is 'z':
			# Extract d/h/m:
			day = time[0:2]
			hour = time[2:4]
			minute = time[4:6]

			# Get the year, month, and seconds from the system:
			ym = datetime.datetime.utcnow().strftime("%Y-%m")
			second = datetime.datetime.utcnow().strftime("%S")

			# Concat ts:
			timestamp = ym + '-' + day + ' ' + hour + ':' + minute + ':' + second

		# If time is in the day-hour-min local format:
		elif time[-1] is '/':
			# Extract d/h/m:
			day = time[0:2]
			hour = time[2:4]
			minute = time[4:6]

			# Get everything else from system, assuming system is in same tz as transmitter...
			ym = datetime.datetime.now().strftime("%Y-%m")
			second = datetime.datetime.now().strftime("%S")

			# Concat ts:
			timestamp = ym + '-' + day + ' ' + hour + ':' + minute + ':' + second + " Local" # I'll handle wonky tzs in C++...

	# Else we can assume we got an instantaneous packet and the current time is the correct time.
	else:
		return datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S Local")

	return timestamp

# Extract "Stupid Lat" (Lat as string in ddmm.mm or compressed formats).
# Takes raw APRS packet as Python string.
# Returns "Stupid Lat" as Python string.
# Evaluating "Stupid Lat" for real lat is done in C++ mostly because I've already written that code.
def getLat(rawPacket):
	unCompPat = re.compile("\\d{4}\\.\\d{2}(N|S)") # Regex pattern for uncompressed lat format.
	compPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}") # Regex pattern for compressed lat format
	
	rawlat = unCompPat.search(rawPacket) # Check if packet is uncompressed
	if rawlat is not None:
		return rawlat.group(0) # If so, return the uncompressed lat string.

	rawlat = compPat.search(rawPacket) # If not uncompressed, check if compressed.
	if rawlat is not None:
		return rawlat.group(0)[-9:-5] # And if compressed, return the compressed lat data.

	return "None" # If null, return empty string for safety.

# Extract "Stupid Lon" (Lon as string in dddmm.mm or compressed formats).
# Takes raw APRS packet as Pythyon string.
# Returns "Stupid Lon" as Python string.
# Again, evaluating "Stupid Lon" into real numbers is done in C++.
def getLon(rawPacket):
	unCompPat = re.compile("\\d{5}\\.\\d{2}(W|E)") # Regex pattern for uncompressed lon format.
	compPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}") # Regex pattern for comprerssed lon format

	rawlon = unCompPat.search(rawPacket) # Check if packet is uncompressed
	if rawlon is not None:
		return rawlon.group(0) # If so, return uncompressed lon string.

	rawlon = compPat.search(rawPacket) # If not uncompressed, check if compressed.
	if rawlon is not None:
		return rawlon.group(0)[-5:-1]

	return "None" # If null, return empty string for safety.

# Extract altitude in feet from raw APRS packet.
# Takes in the raw APRS packet as a Python srting.
# Returns altitude above sea level as a Python int.
def getAlt(rawPacket):
	altPat = re.compile("\\/A=\\d{6}") # Regex pattern for Altitude field in APRS packet.

	rawalt = altPat.search(rawPacket) # Search for and extract the altitude
	if rawalt is not None: # If altitude is found:
		stralt = rawalt.group(0)[3:] # Strip off the "/A=" prefix.
		alt = int(stralt) # Convert to int.
		return alt # Return.

	return -100000 # If this field is missing, return an insane (thus error) value.

# Extract the heading (if present) from the raw APRS packet.
# Takes in the raw APRS packet as a Python string.
# Returns course in degrees from North as a Python int.
def getCse(rawPacket):
	unCompPat = re.compile("(W|E).\\d{3}\\/\\d{3}") # Regex pattern for uncompressed CSE/SPD
	compPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}( |\u00A0|[\x21-\x7A]){3}") # Regex pattern for compressed cse/spd.

	rawcse = unCompPat.search(rawPacket) # Check if packet is uncompressed and has the data.
	if rawcse is not None: # If both above conditions are true,
		strcse = rawcse.group(0)[2:5] # Extract only the course,
		cse = int(strcse) # Turn it into an int,
		return cse # And return it.

	rawcse = compPat.search(rawPacket) # Check if packet is compressed...
	if rawcse is not None: # If so, see if it actually has course/speed.
		if ord(rawcse.group(0)[-1])&8 != 0:
			if ord(rawcse.group(0)[-1])&16 != 0: # This is represented by two bitwise checks.
				return (ord(rawcse.group(0)[-3])-33)*4 # If it does, decompress the course data and return as an int.
				
	return -1 # There can be no data in this field, in which case, return an invalid value as a fail code.

# Extract the speed (if present) from the raw APRS packet.
# Takes in the raw APRS packet as a Python string.
# Returns speed in mph as a Python float.
def getSpd(rawPacket):
	unCompPat = re.compile("(W|E).\\d{3}\\/\\d{3}") # Regex pattern for uncompressed CSE/SPD
	compPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}( |\u00A0|[\x21-\x7A]){3}") # Regex pattern for compressed cse/spd.

	rawspd = unCompPat.search(rawPacket) # Check if packet is uncompressed and has the data.
	if rawspd is not None: # If both of the above are true,
		strspd = rawspd.group(0)[6:] # Extract only the speed,
		return float(strspd)*1.15 # Intify, convert to mph, and return.

	rawspd = compPat.search(rawPacket) # Check if packet is compressed...
	if rawspd is not None: # If so, do the bitwise checks again:
		if ord(rawspd.group(0)[-1])&8 != 0:
			if ord(rawspd.group(0)[-1])&16 != 0:
				exponent = ord(rawspd.group(0)[-2])-33 # If checks pass, decompress, convert, and return.
				knots = math.pow(1.08, float(exponent)) - 1
				return knots*1.15

	return -1.0 # Again, return an impossible value if this fails.

# Extract Comment field.
# This is kind of experimental, since there's no good delimiter for that field.
# This is yet another shortcoming of the APRS format........
# Takes whole packet as Python string, gives back just the comment.
def getCmt(rawPacket):
	unCompPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))\\d{4}\\.\\d\\d(N|S)\\/\\d{5}\\.\\d\\d(W|E).(\\d{3}\\/\\d{3})?") # Regex pattern for /non/-comment part of uncomp string.
	compPat = re.compile("^(cmd:)?[A-Z]{1,2}\\d[A-Z]{2,3}(-\\d{1,2})?>(APT311|APRS),((([A-Z]{1,2}\\d[A-Z]{2,3}(-\\d)?)|(WIDE\\S{1,3})|(q[A-Z]{2}))\\*?,?)+:((\\/|@)\\d{6}(h|z|\\/)|(!|=))(\\/|\\\\)[\x21-\x7E]{9}( |\u00A0|[\x21-\x7A]){3}") # Same for compressed string.

	if unCompPat.match(rawPacket): # If packet matches uncompressed data,
		comment = unCompPat.sub('', rawPacket) # Replace non-comment with empty string to leave only the comment.
		return comment

	if compPat.match(rawPacket): # If matches compressed,
		comment = compPat.sub('', rawPacket) # do the same with that pattern.
		return comment

	return "None" # If null, return empty string for safety.

# Test cases! Uncomment to use!
#print getCmt("W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX")
#print getCallsign("KB3ZZI-12>APRS,WIDE1-1,WIDE2-1:!/:h?/;%EsO   /A=000249|!8|  /KB3ZZI,125,8,27'C,http://habduino.org")
#print getCmt("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\:iOfO   /A=000606|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev")