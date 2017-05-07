/*  Link Telemetry v1.0 "Odyssey"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2017-05-05

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

    KMLWriter.cpp:

    Class to format & write KML files for Google Earth.
    Will allow mapping externally during flight.
    Right now does not use proper XML parser; will fix post-NS-64.
*/

#include "KMLWriter.h"

#include <iostream>

// Constructor
// Default not required for use case; pass in KML filename.
// Opens file, reads previous coordinates, re-prints front matter to file.
BPP::KMLWriter::KMLWriter(std::string _filename) {
    bool coordStart = false;
    std::ifstream prevFile(_filename); // Open file for reading
    if(prevFile.is_open()) {
        while(!prevFile.eof()) { // Read whole file...
            std::string tmpstr;
            std::getline(prevFile, tmpstr);
            if(!coordStart) { // Check if we're reading coordinates yet.
                if(tmpstr.find("<coordinates>") != std::string::npos) { // If we aren't, check to see if we trigger that case
                    coordStart = true; // And set trigger if it is the case!
                }
            } else { // If we are reading coordinates...
                if(tmpstr.find("</coordinates>") == std::string::npos) { // And we haven't hit the end case yet...'
                    points.push_back(tmpstr); // Add the coordinates to our vector!
                } else { // Otherwise...
                    coordStart = false; // Stop reading!
                }
            }
        }
    }
    prevFile.close(); // Close the file.

    // KML header strings
    std::string xmlHead = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n<Document>\n";
    std::string kmlDesc = "<name>Balloon Track</name>\n<Style id=\"gndtrack\">\n<LineStyle>\n<color>da21daff</color>\n<width>4</width>\n</LineStyle>\n<PolyStyle>\n<color>da21da00</color>\n</PolyStyle>\n</Style>\n";
    std::string openPlacemark = "<Placemark>\n<name>Balloon Ground Track</name>\n<styleUrl>#gndtrack</styleUrl>\n";
    std::string openLinestring = "<LineString>\n<extrude>1</extrude>\n<tessellate>1</tessellate>\n<altitudeMode>absolute</altitudeMode>\n<coordinates>\n";

    logFile.open(_filename); // Open file for writing
    logOpen = logFile.is_open(); // Set open status

    // Write out the header
    if(logOpen) {
        logFile << xmlHead << kmlDesc << openPlacemark << openLinestring;
    }
}

// Destructor
// Writes out all the coordinates, then the footer, and closes the file.
BPP::KMLWriter::~KMLWriter() {
    std::string footer = "</coordinates>\n</LineString>\n</Placemark>\n</Document>\n</kml>"; // KML footer string

    // Write out all coordinates and then the footer
    if(logOpen) {
        for(auto s : points) {
            logFile << s << std::endl;
        }
        logFile << footer;
    } else {
        std::cerr << "WARNING: KML output file not opened; writing to file failed!\n";
    }
}

// Add point to vector of coordinates.
// Takes lat, lon, alt, and converts to string.
void BPP::KMLWriter::addPoint(float _lat, float _lon, int _alt) {
    std::string tmpPoint = "";

    tmpPoint += std::to_string(_lon) + ",";
    tmpPoint += std::to_string(_lat) + ",";
    tmpPoint += std::to_string(_alt);

    points.push_back(tmpPoint);
}
