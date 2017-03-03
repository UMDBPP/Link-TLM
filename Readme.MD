Link Telemetry - Version v0.3.0 "Yankee Clipper"
================================================

University of Maryland Space Systems Lab
Code written by Nicholas Rossomando

Link Telemetry (Link-TLM) is designed to be an integrated solution for tracking high-altitude weather balloons
via the [APRS](http://www.aprs.org/) network. The software interfaces with a Kenwood TM-D710GA radio
via a RS-232 serial connection to recieve APRS packets. These packets are then decoded to yield
location information throughout the flight.

The software is currently nearing feature-completion. It currently recieves packets from the radio, parses them,
logs them, and calculates several dervied values including vertical and horizontal speed and downrange distance.
Output is provided via the terminal, and will also be exported to a live-updating JSON file for other processes to
access in the future. Link-TLM has been used in its various versions to track about 15 of the University of Maryland
Nearspace Program's high altitude balloon flights and is now a standard part of the tracking setup.

Dependencies
------------

Link Telemetry's external dependencies are g++ 5.0 or higher, GNU make, and the RapidJson library.
RapidJson is provided with the software (along with a copy of its license; it is MIT, the same as Link-TLM itself).
G++ and GNU make are usually found in a development package for a given distribution; for Debian-based distributions
they are found in `build-essential`, and in Arch they are found in `base-devel` (likely already installed). However,
these packages may not install the latest version of g++. To check what
version is installed, run `g++ --version`. If the version is less than 5.0, as in Linux Mint 17.3, it may
be possible to install a newer version of the compiler. For Mint:

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-5
```

In this case, the `makefile` handles choosing which compiler alias to use.

Building
--------

Building the code is simple thanks to GNU make. Simply open a commmand line, navigate to the `Link-TLM`
directory, and run the command `make`. 

To clean up extra files created during compilation, run `make clean`.
Logs can be purged by `make logclean`, and the executables themselves can be removed by `make purge`.

Configuring
-----------

There are two configuration files for Link-TLM, located in the `Prefs` subdirectory. `settings.json` controls
overall application settings, while `regex.json` defines the regular expressions used to extract data from
recieved APRS packets.

### settings.json ###
-   `balloonCallsigns`: Array of strings; each string is a callsign for a tracker that stays with the balloon for the whole flight (including SSID)
    -   Set to `0` (not in array) if no callsigns defined
-   `vanCallsigns`: Array of strings; each string is a callsign used in one of the chase vans (including SSID)
    -   Set to `0` (not in array) if no callsigns defined
-   `droppedPayloadCallsigns`: Reserved. Not yet implemented.
-   `unparsedLogFilename`: Filename/path for log file of all packets recieved by radio. Raw text file output.
    -   Set to empty string (`""`) if unused. Defaults to `Logs/unparsedPackets.txt`.
-   `parsedLogFilename`: Filename/path for log file of packets parsed by radio. Outputs as CSV, currently with extra timestamp.
    -   Set to empty string (`""`) if unused. Defaults to `Logs/parsedPackets.txt`.
-   `liveOutputFilename`: Reserved. Not yet implemented.

### regex.json ###
**NOTE: All special characters *must* be escaped in this file. Currently, that involves doubling all backslashes
(i.e. `\d` becomes `\\d`)**  
NOTE: All fields *must* contain a valid regular expression

-   `callsignPattern`: Used to extract radio callsign
-   `timestampPattern`: Used to look for a timestamp and extract it if it exists
    -   NOTE: If no timestamp in packet, the time the packet was recieved is used instead.
    -   NOTE: Current BPP trackers *do not* output timestamps.
-   `compressedLatLonPattern`: Used to search for latitude/longitude in APRS compressed format and extract it if it exists
    -   NOTE: Output will be fed to decompressor for APRS format *only*. MIC-E will not be interpreted properly.
-   `uncompressedLatPattern`: Used to search for uncompressed latitude data and extract it if it exists
-   `uncompressedLonPattern`: Used to search for uncompressed longitude data and extract it if it exists
-   `altitudePattern`: Used to extract altitude data
-   `uncompressedCseSpdPattern`: Used to check for and extract course/speed if uncompressed
-   `compressedCseSpdPattern`: Used to check for and extract course/speed if compressed
    -   NOTE: It is possible (and perfectly valid) for neither of the above Cse/Spd fields to exist in an APRS packet
-   `uncompressedCommentPattern`: Used to extract comment from packets that had compressed data
-   `compressedCommentPattern`: Used to extract comment from packets that had uncompressed data

Running
-------

To run the program after building, simply run `./Link-TLM` from a command line in the `Link_TLM` directory.
Enter the filename of the serial port you are using when prompted (something like `/dev/ttyUSB0`). A radio is
not technically necessary; any serial device outputting APRS packets will work. For testing without a radio,
an Arduino writing APRS packets captured on previous launches is used. Output to the screen can be monitored,
and output is also saved to two log files. The log filenames are specified in `settings.json` in the `Prefs/`
subdirectory.

Background Info
---------------

### Why Develop This? ###
The current state of APRS software is fairly poor. None of the software we found were quite able to meet the
Balloon Payload Program's needs. Most significantly, all options avaiable required an internet connection,
which we rarely have while chasing the balloon. Additionally, very few had options for directly interfacing
with a radio; instead, they collect their information from websites like [APRS.fi](http://www.aprs.fi). Thus,
an in-house solution was decided upon.

Forward Path
------------

Link-TLM is nearly feature complete in its current state. Due to several advances in the Nearspace program, we
no longer need a full-fledged mapping solution. Additionally, the always-buggy embedded Python has been stripped
out of the application and replaced with pure C++. This was facilitated by the improvement and widespread compiler
support for C++11's built in regular expressions. Graphing using matplotlib will be replaced by an external
graphing implementation using COSMOS, which is already in use as a ground station in our tracking setup.

Remaining tasks:

-   Implement non-buggy and non-hacky dropped payload tracking
-   Implement chase van tracking
-   Implement JSON data export to COSMOS/tracking internal webserver
-   Revisit and improve radio serial interface

Upon completion of these tasks, Link-TLM will officially be considered feature-complete, and the version number will
be bumped to 1.0.
