/* Link Telemetry v0.3 "Yankee Clipper"
   
   Copyright (c) 2015-2016 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2016-04-15

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

   Plot.cpp:

   Defines functions used for matplotlib wrapper class. Really a go-between to
   enforce one plot in one object.
*/

#include "Plot.h"

#include "PythonInterface.h"

// Default CTOR.
// Initializes values.
BPP::Plot::Plot() {
	plotHandle = 0;
	dividedPlot = false;
	prevX = 0.0f;
	prevY.push_back(0.0f);
}

// Trivial DTOR
BPP::Plot::~Plot() { }

// First initialize plotting (if it isn't).
// Then create this object's plot. 
// Each instance handles one plot.
void BPP::Plot::init(std::string _plotTitle, std::string _xAxisLabel, std::string _yAxisLabel) {
	BPP::PythonInterface::initPlotting(); // Start plotting!
	plotHandle = BPP::PythonInterface::createPlot(_plotTitle, _xAxisLabel, _yAxisLabel); // Create plot and assign handle value.
}

// Divide plot into a number of subplots.
// Up to four supported right now.
void BPP::Plot::dividePlot(int _numberOfDivs) {
	subplotHandles = BPP::PythonInterface::createSubplots(plotHandle, _numberOfDivs); // Set subplot handles for access.
	dividedPlot = true;
}

// Label a subplot. Have to do one at a time - up to the user to keep track of how many subplots there are.
// Subplot number is 1-4; vector handles the actual handles.
void BPP::Plot::labelSubplot(int _subplotNumber, std::string _title, std::string _xAxisLabel, std::string _yAxisLabel) {
	BPP::PythonInterface::formatSubplot(plotHandle, subplotHandles[_subplotNumber - 1], _title, _xAxisLabel, _yAxisLabel);
}

// Add a point to the plot.
// Handles both divided and non-divided cases.
// Vector y needs 1st plot val at index 0, etc.
// X is the INCREMENT to the x axis value, not an absolute value!
void BPP::Plot::plotNewPoint(float _x, std::vector<float> _y) {
	if((prevX == 0.0f) && (prevY[0] == 0.0f)) { // If this is the first point we get, don't plot, just init.
		prevX = _x;
		prevY = _y;
		return;
	}

	if(!dividedPlot) { // If no subplots, simpler call:
		BPP::PythonInterface::plotDataPoint(plotHandle, prevX, prevY[0], prevX + _x, _y[0]);
		prevX += _x; // After plotting, set new prevX and prevY.
		prevY = _y;
	} else { // If subdivided, loop through y and plot in order.
		for(size_t i=0; i < subplotHandles.size(); i++) {
			BPP::PythonInterface::plotDataPoint(plotHandle, subplotHandles[i], prevX, prevY[i], prevX + _x, _y[i]);
		}
		prevX += _x; // Set prev values to new vals.
		prevY = _y;
	}
}
