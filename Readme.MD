Link Telemetry - Version v0.2.0 "Columbia"
====================================================

University of Maryland Space Systems Lab
Code written by Nicholas Rossomando

Link Telemetry (Link-TLM) is designed to be an integrated solution for tracking high-altitude weather balloons
via the [APRS](http://www.aprs.org/) network. The software interfaces with a Kenwood TM-D710GA radio
via a RS-232 serial connection to recieve APRS packets. These packets are then decoded to yield
location information throughout the flight.

This is a very early version of the software. In its current form, it recieves packets from the radio,
logs the raw data, and selectively parses packets from the balloon. The parsed data is displayed to the
terminal and is also logged. Further, the ascent rate of the balloon is calculated from previous packets.
This represents the initial capability of the system. Its objectives were to verify that the radio would
communicate properly with the computer and to verify the accuracy of the APRS packet decoder; both were
completed succesfully.

Additions to the system to reach "version 1.0" include a mapping GUI based on the [Tangram-ES](https://github.com/tangrams/tangram-es)
mapping library, real-time predictions of the balloon's path via the [Habhub](http://predict.habhub.org/)
API, and a port to the Raspberry Pi 2. This effort will include multithreading (and ensuring thread safety).

Dependencies
------------

Link Telemetry's external dependencies are g++ 5.0 or higher, Python 2.7, GNU make, matplotlib and the RapidJson library.
RapidJson is provided with the software (along with a copy of its license; it is MIT, the same as Link-TLM itself).
Most Linux distributions will already have Python installed, however, it may be necessary to install the Python 2
development libraries. The package name varies between distributions; on Debian or Ubuntu based systems, it is 
`python-dev`. G++ and GNU make are usually found in another development package.

Many distributions do not have g++ and make installed by default; for example, under Ubuntu
or Linux Mint, they are found in the `build-essential` package. Under Arch Linux, they are found in the
`base-devel` package. However, these packages may not install the latest version of g++. To check what
version is installed, run `g++ --version`. If the version is less than 5.0, as in Linux Mint 17.3, it may
be possible to install a newer version of the compiler. For Mint:

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-5
```

In this case, the `makefile` handles choosing which compiler alias to use.

Matplotlib must be installed for python 2; this will vary by distribution, but in Ubuntu-based distros, use the following:
`sudo apt-get install python-matplotlib`.

Building
--------

Building the code is simple thanks to GNU make. Simply open a commmand line, navigate to the `Link_TLM`
directory, and run the command `make`. 

To clean up extra files created during compilation, run `make clean`.
Logs can be purged by `make logclean`, and the executables themselves can be removed by `make purge`.

Before running the program, one final step is required. Open the `settings.json` file in the `Prefs/`
subdirectory. Change the `install directory` value to the absolute filepath of the `Link-TLM`
directory. For example, in my development environment, this value would be `"/home/nick/Code/Link-TLM/"`.
This step is necessary for the Python interface; Python does not typically handle relative file paths well.

Running
-------

To run the program after building, simply run `./Link-TLM` from a command line in the `Link_TLM` directory.
Enter the filename of the serial port you are using when prompted (something like `/dev/ttyUSB0`). A radio is
not technically necessary; any serial device outputting APRS packets will work. For testing without a radio,
an Arduino writing APRS packets captured on previous launches is used. Output to the screen can be monitored,
and output is also saved to two log files. The log filenames are specified in `settings.json` in the `Prefs/`
subdirectory.

FAQ
---

### Why Develop This? ###
The current state of APRS software is fairly poor. None of the software we found were quite able to meet the
Balloon Payload Program's needs. Most significantly, all options avaiable required an internet connection,
which we rarely have while chasing the balloon. Additionally, very few had options for directly interfacing
with a radio; instead, they collect their information from websites like [APRS.fi](http://www.aprs.fi). Thus,
an in-house solution was decided upon.

### Why Embed Python? ###
This is an odd choice, perhaps, but there are several reasons for it. Parsing the APRS packets is done via
regular expressions. Python, in my opinion, handles regex better than C++, but was not suited for the entire 
application. Additionally, I was asked to make changing the regex as simple as possible, in case we move to a
tracker which uses an alternate APRS data structure, for example. By using Python, the regexes can be changed
without having to recompile the entire program in a proper build environment.

### Is There More Detail On The Forward Path? ###
Please see `TODO.MD`!
