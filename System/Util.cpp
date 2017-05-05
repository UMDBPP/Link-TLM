/* Link Telemetry v1.0 "Odyssey"
   
   Copyright (c) 2015-2017 University of Maryland Space Systems Lab
   NearSpace Balloon Payload Program
   
   Written by Nicholas Rossomando
   2015-10-26

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

   Util.cpp:

   Defines utilities from Util.h.
*/

#include "Util.h"

#define _USE_MATH_DEFINES // For M_PI

#include <unistd.h>
#include <term.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cmath>
#include <iostream>

// Clear the terminal screen.
// Uses Linux terminal libs to avoid system calls and/or quick hacks.
void BPP::clearTerm() {
    if(!cur_term) {
        int result;
        setupterm(NULL, STDOUT_FILENO, &result);
        if(result <= 0) {
            return;
        }
    }

    putp(tigetstr("clear"));
}

// Print program banner.
void BPP::LinkTlm() {
    std::cout << ".##.......####.##....##.##....##.........########.##.......##.....##\n";
    std::cout << ".##........##..###...##.##...##.............##....##.......###...###\n";
    std::cout << ".##........##..####..##.##..##..............##....##.......####.####\n";
    std::cout << ".##........##..##.##.##.#####....#######....##....##.......##.###.##\n";
    std::cout << ".##........##..##..####.##..##..............##....##.......##.....##\n";
    std::cout << ".##........##..##...###.##...##.............##....##.......##.....##\n";
    std::cout << ".########.####.##....##.##....##............##....########.##.....##\n";
    std::cout << std::endl << std::endl;
}

// Degrees to radians.
// Used for distance calculation stuff.
float BPP::deg2rad(float _degrees) {
    return _degrees * (M_PI/180.0f);
}

// Check if a directory exits.
// If not, create it.
// Used to prevent logs from not opening if the directory doesn't exist.
void BPP::createDir(std::string _dirName) {
    struct stat st;

    std::string searchString = "./" + _dirName;

    if(stat(searchString.c_str(), &st) == 0) { // The string exists...
        if((st.st_mode & S_IFDIR) != 0) {
            return; // If it is a directory, silently return.
        } else {
            std::cerr << "Warning! " << _dirName << " exists as a file, not a Directory! File saving may not work!\n";
            return; // If it's a file, throw a warning...
        }
    } else {
        mkdir(searchString.c_str(), 0755); // If nothing exists, make the directory.
    }
}
