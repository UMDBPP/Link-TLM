CFLAGS=--std=c++14 -c -Wall -Wextra -O2 -I. -I./DataStructures -I/usr/include/python2.7 -I./deps
LIBS=-L/usr/lib/python2.7/config -lpython2.7 -lpthread -ldl -lutil -lm -lcurses -Xlinker -export-dynamic

# Set compiler alias based on version:
GCC_MAJOR_VERSION=$(shell g++ --version | grep [0-9].[0-9].[0-9] -o | grep ^[0-9] -o)
ifeq ($(GCC_MAJOR_VERSION),5)
	CC=g++
else
	CC=g++-5
endif

all: main.o MainProcess.o PythonInterface.o rs232.o Util.o Packet.o Log.o JSONLoader.o
	$(CC) main.o MainProcess.o PythonInterface.o rs232.o Util.o Packet.o Log.o JSONLoader.o -o Link-TLM $(LIBS)

test: testmain.o MainProcess.o PythonInterface.o rs232.o Util.o Packet.o Log.o JSONLoader.o
	$(CC) testmain.o MainProcess.o PythonInterface.o rs232.o Util.o Packet.o Log.o JSONLoader.o -o TestBin $(LIBS)

main.o: main.cpp
	$(CC) main.cpp $(CFLAGS)

testmain.o: testmain.cpp
	$(CC) testmain.cpp $(CFLAGS)

MainProcess.o: MainProcess.cpp
	$(CC) MainProcess.cpp $(CFLAGS)

PythonInterface.o: Interface/PythonInterface.cpp
	$(CC) Interface/PythonInterface.cpp $(CFLAGS)

rs232.o: rs232.c
	$(CC) rs232.c $(CFLAGS)

Util.o: System/Util.cpp
	$(CC) System/Util.cpp $(CFLAGS)

Packet.o: DataStructures/Packet.cpp
	$(CC) DataStructures/Packet.cpp $(CFLAGS)

Log.o: System/Log.cpp
	$(CC) System/Log.cpp $(CFLAGS)

JSONLoader.o: System/JSONLoader.cpp
	$(CC) System/JSONLoader.cpp $(CFLAGS)

clean:
	rm -rf *.o TestBin

logclean:
	rm -rf test.txt Logs/*

purge:
	rm -rf *.o TestBin Link-TLM

.PHONY: clean logclean purge
