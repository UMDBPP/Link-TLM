CFLAGS=--std=c++14 -c -Wall -Wextra -O2 -I. -I./DataStructures -I./deps
LIBS=-lpthread -lm -lcurses

# Set compiler alias based on version:
GCC_MAJOR_VERSION=$(shell g++ --version | grep -E "[0-9][[:punct:]][0-9][[:punct:]][0-9]" -o | grep "^[0-9]" -o)
ifneq ($(GCC_MAJOR_VERSION),4)
	CC=g++
else
	CC=g++-5
endif

all: main.o MainProcess.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o Parser.o
	$(CC) main.o MainProcess.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o Parser.o -o Link-TLM $(LIBS)

test: test.o MainProcess.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o Parser.o
	$(CC) test.o MainProcess.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o Parser.o -o TestLink $(LIBS)

main.o: Main/main.cpp
	$(CC) Main/main.cpp $(CFLAGS)

test.o: Tests/test.cpp
	$(CC) Tests/test.cpp $(CFLAGS)

MainProcess.o: Main/MainProcess.cpp
	$(CC) Main/MainProcess.cpp $(CFLAGS)

RS232.o: Interface/RS232.cpp
	$(CC) Interface/RS232.cpp $(CFLAGS)

Util.o: System/Util.cpp
	$(CC) System/Util.cpp $(CFLAGS)

Packet.o: DataStructures/Packet.cpp
	$(CC) DataStructures/Packet.cpp $(CFLAGS)

GroundTrack.o: DataStructures/GroundTrack.cpp
	$(CC) DataStructures/GroundTrack.cpp $(CFLAGS)

Log.o: System/Log.cpp
	$(CC) System/Log.cpp $(CFLAGS)

JSONLoader.o: System/JSONLoader.cpp
	$(CC) System/JSONLoader.cpp $(CFLAGS)

Parser.o: Parser/Parser.cpp
	$(CC) Parser/Parser.cpp $(CFLAGS)

clean:
	rm -rf *.o TestBin test.txt

logclean:
	rm -rf test.txt Logs/*

purge:
	rm -rf *.o TestLink Link-TLM test.txt

.PHONY: clean logclean purge
