CFLAGS=--std=c++14 -c -Wall -Wextra -O2 -I. -I./DataStructures -I/usr/include/python2.7 -I./deps
LIBS=-L/usr/lib/python2.7/config -lpython2.7 -lpthread -ldl -lutil -lm -lcurses -Xlinker -export-dynamic

# Set compiler alias based on version:
GCC_MAJOR_VERSION=$(shell g++ --version | grep -E "[0-9][[:punct:]][0-9][[:punct:]][0-9]\s?" -o | grep "^[0-9]" -o)
ifeq ($(GCC_MAJOR_VERSION),5)
	CC=g++
else
	CC=g++-5
endif

all: main.o MainProcess.o PythonInterface.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o
	$(CC) main.o MainProcess.o PythonInterface.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o -o Link-TLM $(LIBS)

test: testmain.o MainProcess.o PythonInterface.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o
	$(CC) testmain.o MainProcess.o PythonInterface.o RS232.o Util.o Packet.o GroundTrack.o Log.o JSONLoader.o -o TestBin $(LIBS)

main.o: main.cpp
	$(CC) main.cpp $(CFLAGS)

testmain.o: testmain.cpp
	$(CC) testmain.cpp $(CFLAGS)

MainProcess.o: MainProcess.cpp
	$(CC) MainProcess.cpp $(CFLAGS)

PythonInterface.o: Interface/PythonInterface.cpp
	$(CC) Interface/PythonInterface.cpp $(CFLAGS)

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

clean:
	rm -rf *.o TestBin test.txt

logclean:
	rm -rf test.txt Logs/*

purge:
	rm -rf *.o TestBin Link-TLM test.txt

.PHONY: clean logclean purge
