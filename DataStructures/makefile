CFLAGS=--std=c++14 -c -Wall -Wextra -O2 -I. -I.. -I../deps -DRAPIDJSON_HAS_STDSTRING=1

# Set compiler alias based on version:
GCC_MAJOR_VERSION=$(shell g++ --version | grep -E "[0-9][[:punct:]][0-9][[:punct:]][0-9]" -o | grep "^[0-9]" -o)
ifneq ($(GCC_MAJOR_VERSION),4)
	CC=g++
else
	CC=g++-5
endif

all: GroundTrack.o Packet.o

GroundTrack.o: GroundTrack.cpp
	$(CC) GroundTrack.cpp $(CFLAGS)

Packet.o: Packet.cpp
	$(CC) Packet.cpp $(CFLAGS)
