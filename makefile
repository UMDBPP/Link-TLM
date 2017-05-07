CFLAGS=--std=c++14 -c -Wall -Wextra -O2 -I. -I./deps -DRAPIDJSON_HAS_STDSTRING=1
LIBS=-lpthread -lm -lcurses

# Set compiler alias based on version:
GCC_MAJOR_VERSION=$(shell g++ --version | grep -E "[0-9][[:punct:]][0-9][[:punct:]][0-9]" -o | grep "^[0-9]" -o)
ifneq ($(GCC_MAJOR_VERSION),4)
	CC=g++
else
	CC=g++-5
endif

all: main datastructures interface output parser system
	$(CC) Main/*.o DataStructures/*.o Interface/*.o Output/*.o Parser/*.o System/*.o -o Link-TLM $(LIBS)

test: datastructures interface output parser system
	cd Tests && $(MAKE)
	$(CC) Tests/*.o DataStructures/*.o Interface/*.o Output/*.o Parser/*.o System/*.o -o TestLink $(LIBS)

main:
	cd Main && $(MAKE)

datastructures:
	cd DataStructures && $(MAKE)

interface:
	cd Interface && $(MAKE)

output:
	cd Output && $(MAKE)

parser:
	cd Parser && $(MAKE)

system:
	cd System && $(MAKE)

clean:
	rm -rf *.o */*.o TestLink test.json

logclean:
	rm -rf test.txt Logs/*

purge:
	rm -rf *.o */*.o TestLink Link-TLM test.*

.PHONY: clean logclean purge
