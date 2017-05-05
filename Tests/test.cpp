/*  Link Telemetry v0.3 "Yankee Clipper"
    
    Copyright (c) 2015-2017 University of Maryland Space Systems Lab
    NearSpace Balloon Payload Program
    
    Written by Nicholas Rossomando
    2017-03-03

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

    test.cpp:

    Main process for testing framework.
    Tests all components of Link-TLM independently.
    Essentially lightweight unit testing.
*/

#include <iostream>
#include <string>

#include "TestTemplate.h"
#include "DataStructureTests.h"

#include "Output/JSONWriter.h"

void write_test_json(int a, float b);

int main(void) {
    test_DecodedPacket();
    test_Packet();

    write_test_json(1,2.65f);
    write_test_json(2,7.68f);
    write_test_json(5,13.4432f);

    return 0;
}

void write_test_json(int a, float b) {
    BPP::JSONWriter out("test.json");
    out.addValue("stringVal", "testString");
    out.addValue("someInt", a);
    out.addValue("someFloat", b);
}
