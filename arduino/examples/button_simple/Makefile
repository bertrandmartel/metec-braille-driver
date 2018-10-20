OBJECTS=examples/button_simple/button_simple.o
HEADERS=-I../include
STATIC_LIB=libmetecdriver.a

#uncomment this for arduino Mini
#BOARD=-DARDUINO_AVR_MINI
#export BOARD

PORT=/dev/ttyACM0
export PORT

export OBJECTS
export HEADERS
export STATIC_LIB

.PHONY: all

all: lib
	$(MAKE) -C ../../arduino-makefile
lib:
	$(MAKE) -C ../.. all
clean:
	$(MAKE) -C ../../arduino-makefile clean

distclean:
	$(MAKE) -C ../../arduino-makefile distclean