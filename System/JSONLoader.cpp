/*  Link Telemetry v0.2.0 "Columbia"
    
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

    JSONLoader.cpp:

    Implementation of JSON loader with RapidJson. Includes error checking.

    Settings in file:
    - Balloon Callsigns
    - Van Callsigns
    - Log File Names
    - Install Directory
*/

#include "JSONLoader.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <rapidjson/error/en.h>

// Constructor
// Takes filename, opens file, dumps all file data into a string.
BPP::JSONLoader::JSONLoader(const std::string& _jsonFileName) {
    loadFailure = false;

    std::ifstream json(_jsonFileName); // Pop open the json file.
    if(json.fail()) { // Error check!
        std::cerr << "Settings File " << _jsonFileName << "Not Found! Using Assumed Standards!\n";
        loadFailure = true;
    } else { // If no failure, continue:
        std::string jsonFileString = ""; // String for entire JSON file.
        std::string line; // Temp variable for getline()
        while(std::getline(json, line)) { // Read all data from file.
            jsonFileString += line; // JSON doesn't care about newlines, so just concat
        }

        const char* jsonCStr = jsonFileString.c_str(); // RapidJson needs a C string.
        if(settings.Parse(jsonCStr).HasParseError()) { // Parse the string into the DOM object.
            std::cerr << "Parsing Error in " << _jsonFileName << ", Using Assumed Standards!\n";
            loadFailure = true;
        } else if(!settings.IsObject()) { // Further error checking.
            std::cerr << "JSON error - DOM is not an Object. Using Assumed Standard Settings!\n";
            loadFailure = true;
        }
    }

}

// Trivial destructor.
BPP::JSONLoader::~JSONLoader() { }

// Parse the balloon callsigns from the JSON settings file.
// Returns a vector of all supplied callsigns.
// If this fails or if JSON didn't load, use W3EAX-9 as a standard.
std::vector<std::string> BPP::JSONLoader::getBalloonCalls() {
    std::vector<std::string> callsigns; // Return value

    if(loadFailure || !settings.HasMember("balloonCallsigns")) { // If balloon callsigns aren't defined or the JSON fails to load,
        std::cerr << "ERROR: Settings File Does Not Contain Balloon Callsign Definitions. Using W3EAX-9.\n"; // Throw an error message,
        callsigns.push_back(std::string("W3EAX-9")); // Use the default value,
        return callsigns; // And return.
    }

    // Get the value stored in "balloonCallsigns":
    const rapidjson::Value& balloonCalls = settings["balloonCallsigns"];

    // Now, check if the defined callsigns are an array or a single string:
    if(balloonCalls.IsArray()) { // If an array of callsigns was specified
        for(rapidjson::SizeType i=0; i<balloonCalls.Size(); i++) { // Dump them all into the vector.
            callsigns.push_back(balloonCalls[i].GetString());
        }
    } else if (balloonCalls.IsString()) {
        callsigns.push_back(balloonCalls.GetString()); // If only one CS was specified (as a string), just add it to the vector.
    } else {
        // If we got here, a value was specified but it was NOT in the correct format, assume default.
        std::cerr << "ERROR: Balloon Callsign Definitions in Unrecognized Format. Using W3EAX-9.\n";
        callsigns.push_back(std::string("W3EAX-9"));
    }

    return callsigns; // Finally, return.
}

// Literally the same as the above, except for grabbing callsigns assigned to vans.
// This parameter is non-critical, so no default value is assumed.
// Returns an empty vector if there are issues.
std::vector<std::string> BPP::JSONLoader::getVanCalls() {
    std::vector<std::string> callsigns; // Return value.

    if(loadFailure || !settings.HasMember("vanCallsigns")) { // If calls aren't specified at all,
        std::cerr << "WARNING: No Van Callsigns Defined. Van Tracking Disabled!\n";
        return callsigns; // Return an empty vector.
    }

    const rapidjson::Value& vanCalls = settings["vanCallsigns"]; // Extract values stored in "vanCallsigns"

    // Do the same string/array check as above; except in case of error, return empty vector.
    if(vanCalls.IsArray()) {
        for(rapidjson::SizeType i=0; i<vanCalls.Size(); i++) {
            callsigns.push_back(vanCalls[i].GetString());
        }
    } else if (vanCalls.IsString()) {
        callsigns.push_back(vanCalls.GetString());
    } else {
        // Still throw an error message here, but lower priority and no default assumed.
        std::cerr << "WARNING: Van Callsign Definitions in Unrecognized Format. Van Tracking Disabled!\n";
    }

    return callsigns; // Return.
}

// Gets the filename for the log file of all recieved packets.
// This log stores the packets in an unparsed format.
// Assumes a standard of "unparsedPackets.txt" if none specified.
std::string BPP::JSONLoader::getUnparsedLogFile() {
    std::string filename = "Logs/unparsedPackets.txt"; // return value.

    // Check for JSON failure and data existence. Use default if either fail.
    if(loadFailure || !settings.HasMember("unparsedLogFilename")) {
        std::cerr << "WARNING: No Unparsed Log Filename Specified. Using Logs/unparsedPackets.txt\n";
        return filename;
    }

    const rapidjson::Value& unparsedName = settings["unparsedLogFilename"]; // Extract value.

    if(unparsedName.IsString()) {
        filename = unparsedName.GetString(); // If we have a good data type, get the value.
    } // Otherwise, the default is automatically used.

    return filename; // And return.
}

// Gets the filename for the parsed packet log file.
// This log stores only packets that have been parsed
// I.E. only packets that came from the balloon or our vans.
// Assumes a standard of "parsedPackets.txt" if none specified.
std::string BPP::JSONLoader::getParsedLogFile() {
    std::string filename = "Logs/parsedPackets.txt"; // return value.

    // Now do the exact same process as above, just with a different DOM element.
    if(loadFailure || !settings.HasMember("parsedLogFilename")) {
        std::cerr << "WARNING: No Parsed Log Filename Specified. Using Logs/parsedPackets.txt\n";
        return filename;
    }

    const rapidjson::Value& parsedName = settings["parsedLogFilename"]; 

    if(parsedName.IsString()) {
        filename = parsedName.GetString();
    }

    return filename; // And return.
}

// Get the install directory of the program.
// This is used for the Python scripts.
// Python doesn't deal with relative file paths well.
std::string BPP::JSONLoader::getInstallDirectory() {
    std::string directory = "/home/nick/Code/Link_TLM"; // Initialize to default (build env)

    // Again, read & error check.
    if(loadFailure || !settings.HasMember("installDirectory")) {
        std::cerr << "WARNING: No Install Directory Specified. Using /home/nick/Code/Link_TLM.\n";
        return directory;
    }

    const rapidjson::Value& parsedName = settings["installDirectory"]; 

    if(parsedName.IsString()) {
        directory = parsedName.GetString();
    }

    return directory; // And return.
}
