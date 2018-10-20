# This makefile build the library only
# generates libbraille.a static library
OBJECTS=../src/braille.o ../src/pch.o
HEADERS=-I../include

PORT=/dev/ttyACM0
export PORT

export OBJECTS
export HEADERS

.PHONY: all

all:
	$(MAKE) -C arduino-makefile libmetecdriver.a

clean:
	$(MAKE) -C arduino-makefile clean

distclean:
	$(MAKE) -C arduino-makefile distclean