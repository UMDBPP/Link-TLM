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

   Plot.h:

   Declares plotting class. Keeps track of individual plots and data on them.
   Used with matplotlib wrapper found in PythonInterface.
*/

#ifndef PLOT_H
#define PLOT_H

#include <string>
#include <vector>

namespace BPP {

class Plot {

	private:
		int plotHandle; // Handle to Matplotlib figure.
		std::vector<int> subplotHandles; // Handles to each subplot (if they exist).
		bool dividedPlot; // Check for if we're subplotted or not (so we don't have to check if the vector is empty).

		// "Old" values - for line plotting.
		float prevX;
		std::vector<float> prevY;

	public:
		Plot(); // CTOR and DTOR
		~Plot();

		void init(std::string _plotTitle, std::string _xAxisLabel, std::string _yAxisLabel); // Create the plot and label it.
		void dividePlot(int _numberOfDivs); // Divide plot into subplots.
		void labelSubplot(int _subplotNumber, std::string _title, std::string _xAxisLabel, std::string _yAxisLabel); // Label any subplot.

		void plotNewPoint(float _x, std::vector<float> _y); // Add a new point to each subplot (or just the plot if only one.)

}; // Plot

} // BPP

#endif
// PLOT_H
