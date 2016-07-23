''' Link Telemetry v0.3 "Yankee Clipper"

Copyright (c) 2015-2016 University of Maryland Space Systems Lab
NearSpace Balloon Program

Written by Nicholas Rossomando
2016-04-14

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

BPPplotter.py:

Wrapper for matplotlib. Each function has a corresponding C++ call.
Rationale: There aren't any good C++ plotting libraries...
'''

# Bring in matplotlib
from matplotlib import pyplot as plt

# Keep track of total figures:
figs = 0

# Set up general plotting stuff we need.
def initInteractivePlot():
	plt.ion() # Go into interactive mode.

# Create a plot.
# Label the plot, label the axes.
# Return the integer handle of the figure.
def createPlot(Title, xLabel, yLabel):
	global figs
	figs += 1
	plt.figure(figs) # Create the figure handle

	plt.title(Title) # Title the plot (will be wiped if subplot used.)
	plt.xlabel(xLabel) # Label the x axis
	plt.ylabel(yLabel) # Label the y axis

	plt.draw() # Update the plot.

	return figs # This will report figure handle to C++ (or Python, honestly.)

# Create 2-4 subplots; do nothing if other numbers requested.
# Doesn't return handles; C++ takes care of that since there are limited possiblities.
def createSubplots(figHandle, numPlots):
	plt.figure(figHandle) # Switch to proper figure

	if numPlots is 1:
		return
	elif numPlots is 2:
		plt.subplot(211) # Create 2 subplots, keep top one active
	elif numPlots is 3:
		plt.subplot(311) # Create 3, keep top active
	elif numPlots is 4:
		plt.subplot(221) # Create 4, two row two col, keep upper left active.
	else:
		return

	plt.draw() # Update the plot.

# Allows titling and axis labelling of subplots.
def formatSubplot(figHandle, subplotHandle, Title, xLabel, yLabel):
	plt.figure(figHandle)
	plt.subplot(subplotHandle) # Switch the the figure and subplot we want to edit.

	plt.title(Title)
	plt.xlabel(xLabel)
	plt.ylabel(yLabel) # Add all our labels.

	plt.draw() # Update plot

# Add a point to the plot. New values are x2, y2.
# Previous points are x1, y1.
# This allows plotting as a line instead of points (thankfully).
# For non-subplotted graphs, use 111 as subplotHandle.
def addPointToPlot(figHandle, subplotHandle, x1, x2, y1, y2):
	plt.figure(figHandle)
	plt.subplot(subplotHandle) # Switch to desired plot & subplot

	plt.plot([x1, x2],[y1, y2],'g') # Draw a line between the old and new points.
	plt.draw() # Make explicit draw() call.
