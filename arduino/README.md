# Arduino

Build library only :
```bash
cd arduino
make
```

To build the examples, go to the example directory you want and build, for instance :
```bash
cd arduino/examples/buttons
make
```

In each example folder, `make` will also upload to Arduino using `avrdude`

## Write to multiple Braille cells

```cpp
#include "braille.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         6
#define DIN        3
#define STROBE     4
#define CLK        5
#define DOUT       2

#define PATTERN_LENGTH 14
uint16_t pattern[PATTERN_LENGTH] = {
    0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
//    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
//    W         E        L        C        O        M        E             H       O        M       E             !
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

int main() {
    init();
    metecDriver.init();
    metecDriver.writeCells(pattern, PATTERN_LENGTH, false);
    while (1)
        delay(50);
    return 0;
}
```

## Write to single Braille cell

```cpp
#include "braille.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         6
#define DIN        3
#define STROBE     4
#define CLK        5
#define DOUT       2

#define PATTERN_LENGTH 11
uint16_t pattern[PATTERN_LENGTH] = {
    0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
//     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
//     H       E      L       L        O             W       O       R      L       D
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

int main() {
    init();
    metecDriver.init();
    for (uint8_t i = 0 ; i  < PATTERN_LENGTH; i++) {
        metecDriver.writeSingleCell(CELL_COUNT - 1 - i, pattern[i]);
    }
    while (1)
        delay(300);
    return 0;
}
```

## Get button events

```cpp
#include "braille.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         6
#define DIN        3
#define STROBE     4
#define CLK        5
#define DOUT       2

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

int main() {
    init();
    metecDriver.init();
    Serial.begin(9600);
    while (1) {
        metecDriver.checkButton();
        if (metecDriver.btn_update) {
            Serial.print("button ");
            Serial.print(metecDriver.btn_position);
            Serial.print(" switched to ");
            Serial.println((int)metecDriver.btn_state, DEC);
            metecDriver.btn_update = false;
        }
        delay(50);
    }
    return 0;
}
```

## Arduino development environment

```bash
cd project_dir
git submodule add git://github.com/bertrandmartel/metec-braille-driver.git
git submodule update --init --recursive
```

Create the following Makefile : 

```makefile
OBJECTS=main.o
HEADERS=-Imetec-braille-driver/include
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
	$(MAKE) -C metec-braille-driver/arduino/arduino-makefile
lib:
	$(MAKE) -C metec-braille-driver/arduino all
clean:
	$(MAKE) -C metec-braille-driver/arduino/arduino-makefile clean

distclean:
	$(MAKE) -C metec-braille-driver/arduino/arduino-makefile distclean
```

In he makefile above, it includes the object `main.o` assuming you have a standard `main.cpp` with a main function

## License

The MIT License (MIT) Copyright (c) 2018 Bertrand Martel