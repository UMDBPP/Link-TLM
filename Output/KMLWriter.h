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

    KMLWriter.h:

    Class to format & write KML files for Google Earth.
    Works in same "one-shot" manner as JSONWriter.
    Will allow mapping externally during flight.
*/

#ifndef KML_H
#define KML_H

#include <string>
#include <vector>
#include <fstream>

namespace BPP {

class KMLWriter {

    public:
        KMLWriter(std::string _filename);
        ~KMLWriter();

        void addPoint(float _lat, float _lon, int _alt);

    private:
        std::vector<std::string> points;
        std::ofstream logFile;
        bool logOpen;

}; // KMLWriter

} // BPP

#endif // KML_H
