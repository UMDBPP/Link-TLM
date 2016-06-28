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
    jsonFilename = _jsonFileName;

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

// Parse out a vector (array) of strings from a JSON file.
// Argument is the name of the JSON member to search for.
// Return a vector of all supplied strings; if only one string, return length 1 vector.
// Also print error stuff!
std::vector<std::string> BPP::JSONLoader::getStringVector(const std::string& _jsonMember) {
    std::vector<std::string> values; // Returned values

    if(loadFailure || !settings.HasMember(_jsonMember.c_str())) { // If the member speicifed wasn't found,
        std::cerr << "ERROR: JSON file " << jsonFilename << " has no member " << _jsonMember << "! Returning Empty!\n"; // Report the error,
        return values; // and return (with an empty vector).
    }

    // If we've gotten this far, the value exists, so store it:
    const rapidjson::Value& jsonValues = settings[_jsonMember.c_str()];

    // Check for array vs. single string vs. unexpected data type:
    if(jsonValues.IsArray()) {
        if(jsonValues.Size() == 0) { // Throw specific error for size zero array:
            std::cerr << "WARNING: Value " << _jsonMember << " in file " << jsonFilename << " is an Empty array!\n";
        } else if(jsonValues[0].IsString()) { // Needs to be an array of strings!
            for(rapidjson::SizeType i=0; i<jsonValues.Size(); i++) {
                values.push_back(jsonValues[i].GetString());
            }
        }
    } else if(jsonValues.IsString()) {
        values.push_back(jsonValues.GetString());
    } else {
        // If we got down here, we have some data type we don't want.
        std::cerr << "ERROR: Value " << _jsonMember << " in file " << jsonFilename << " in Unexpected format!\n";
        std::cerr << "Ensure that " << _jsonMember << " is a string or array of strings!\n";
    }

    return values; // Return even if empty; check if empty from calling function for safety.
}

// Parse out a string from a JSON file.
// Argument is the name of the JSON member to search for.
// Simply returns the string.
// Prints error messages, as well.
std::string BPP::JSONLoader::getString(const std::string& _jsonMember) {
    std::string value = "";

    // Now do the exact same process as above, just with a different DOM element.
    if(loadFailure || !settings.HasMember(_jsonMember.c_str())) {
        std::cerr << "ERROR: JSON file " << jsonFilename << " has no member " << _jsonMember << "! Returning empty string!\n";
        return value;
    }

    const rapidjson::Value& jsonValue = settings[_jsonMember.c_str()]; 

    if(jsonValue.IsString()) {
        value = jsonValue.GetString();
    } else {
        std::cerr << "ERROR: Value " << _jsonMember << " in file " << jsonFilename << " in Unexpected format!\n";
        std::cerr << "ensure that " << _jsonMember << " is a string!\n";
    }

    return value; // And return.
}
