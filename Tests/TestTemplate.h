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

    TestTemplate.h:

    Templates for generic testing of code elements.
    Essentially a lightweight unit test setup.
*/

#ifndef TEST_H
#define TEST_H

// Template for a simple value check test for any type
// _value: value to be checked, _correctVal: value expected, _testName: name of the test running (useful for output)
// Return 0 for success and 1 for failure
template<typename T> int test_value(T _value, T _correctVal, std::string _testName = "GENERIC TEST") {
    if(_value == _correctVal) {
        return 0;
    }

    std::cerr << "\e[1m\e[31mFAILED TEST: \e[0m" << _testName << std::endl;
    return 1;
}

#endif // TEST_H
