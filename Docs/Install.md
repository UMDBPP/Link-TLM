# Link-TLM Installation #

Welcome to `Link-TLM`! If you're an experienced user/developer of `Link-TLM`, you can skip ahead the the [Dependencies](#dependencies) section.
If not, greetings new BPP tracker and/or developer! This here guide will help you get `Link-TLM` built & running for the first time.

## Getting Started ##

First of all: `Link-TLM` is a Linux-only program. It will not build on Windows or Mac. If you are not on a Linux computer, stop reading and get access to one!
There are two Linux laptops in the balloon lab that have been set up specifically for tracking purposes. A VM (Virtual Machine) won't work, as `Link-TLM` needs
USB port access, and typically setting that up on a VM is a massive pain. (If you manage to get it, please update this doc with instructions!)

If you are wholly unfamiliar with Linux, I suggest you learn at least the basics of using the Linux command line - it will make this process much easier.
In particular, the `cd`, `ls`, `mv`, `cp`, and `rm` commands are probably the most important. Also learn about `git`; `apt-get` would be good, too. (`apt-get`
is not universal to Linux, but the Lab laptops run a version of Linux, called Debian, which uses it.)

If you are familiar with Linux, great!

`Link-TLM` is written in C++11, with support scripts written in Python. I specify C++11 because it was a huge update to the language, and some older compilers
don't support it yet - detail is below. C++ was chosen because, at the time, I viewed it as the right tool for the job, and I actually quite like C++. In
hindsight, Python might have been okay, but I do think I had a better time of it using C++. Larger Python projects, in my experience, tend to get both slow
and hacked together; and the "Python Way" sometimes gets _in the way_ of what you want to do. However, for the support scripts, Python is the perfect choice.
GUI programming is one area where C++ is terribly weak, and Python is quite good, so the trade is in Python's favor there. This brief ramble hopefully provides
some insight into my choice of language for those who care about such things!

From here on out, **I will assume you are using Debian as your Linux distribution**. Ubuntu should be very 
similar in most cases. Arch and other rolling release distros should also be easy to get going; only 
_very_ "stable" (outdated) setups such as Red Hat/CentOS will really have issues.

## Dependencies ##

`Link-TLM` itself is fairly light on dependncies. It needs any version of `g++` newer than 5.0, `make`, 
the rapidjson library, and possibly `ncurses`. The last one varies from distro to distro; install it just 
to be safe. Also, needless to say, you'll need `git` to get the code from Github!

Before installing dependencies, it may be wise to `sudo apt-get update && sudo apt-get upgrade`.

- `g++` and `make` can be found in the `build-essential` package: `sudo apt-get install build-essential`
- rapidjson is included with `Link-TLM`, so no installation is required!
- `ncurses` can be found in the `ncurses-dev` package: `sudo apt-get install ncurses-dev`
- `git` is as easy as `sudo apt-get install git`

**WARNING**: If you are running a Linux distribution which has not updated in a long time (like CentOS), 
you will likely find that you are _way_ behind on compiler support. You may need to check your compiler 
version and install a newer version manually. The build process tries to figure out using g++ 5 if you 
have multiple versions, but it's kinda hacky and never really worked right.

`Link-TLM-Configurator` and other Python GUI scripts require `wxPython` to run. `wxPython` is a fantastic
GUI library - it is fairly modular and simple to use, and it integrates with GTK desktop themes. 
Unfortunately, it requires compilation from source at the moment. This is easy, but can take quite a while.
On my desktop processor (i5-6600k), it took ~15 minutes; on a single core VM it took ~45. Figure somewhere 
in between those two is a good estimate.

To build `wxPython`:

- Ensure `pip3` (`pip` for Python 3) is installed: `sudo apt-get install python3-pip`
- Install build dependencies for `wxPython` (these are unfortunately large...)
    - `sudo apt-get install -y libgtk2.0-dev libgtk-3-dev libjpeg-dev libtiff-dev libsdl1.2-dev`
    - `sudo apt-get install -y libgstreamer-plugins-base0.10-dev libnotify-dev freeglut3 freeglut3-dev`
    - `sudo apt-get install -y libsm-dev libwebkitgtk-dev libwebkitgtk-3.0-dev python3-dev`
    - The above can all be combined into one command if you're so inclined
- Install `wxPython` from source: `sudo pip3 install wxpython`
    - This is the part that will take a while. Let it go while you do something else and check every 5 minutes.

## Getting `Link-TLM` from Git ##

To get `Link-TLM` from the program's Github organization, simply run:

`git clone https://www.github.com/umdbpp/Link-TLM`

This will create a `Link-TLM/` directory in your current path (so if you ran from you home directory, 
you'd have `~/Link-TLM`). The `master` branch of the git repository will be downloaded into the directory.
**`master` is stable and is the branch you should use to track actual flights.** If you are developing,
it's best to do your work in another branch. (If you're not familiar with how `git`branching works, I
strongly suggest looking it up; you'll get better information than I can condense here.)

You can use `git status` to check your copy of `Link-TLM` against the "remote" copy on Github. `git status`
tells you what branch you're on, if there are updates on the remote copy, and if there are changes you 
have made but not committed. If there are remote changes, you can `git pull` to get up to date.

## Building `Link-TLM` ##

First things first, `cd` into the `Link-TLM` directory that the git clone created. This directory is where 
you will do basically all things `Link-TLM`.

As a Python script, `Link-TLM-Configurator` is ready to go out of the box (as long as you installed 
dependencies as required above.)

To build the C++ code, run `make`. On a multi-core processor, `make -j <number of cpus>` will speed the
process up, but it overall only takes a few seconds to build anyway.

The build process leaves a bunch of object files lying around. To clean them up, run `make clean`.

If you want to start from scratch, `make purge` will also remove all the compiled executables.

You should now have a working copy of `Link-TLM` built and ready to go!

_Where to next? You should read [Configuring Link-TLM](./Configuration.md)_
