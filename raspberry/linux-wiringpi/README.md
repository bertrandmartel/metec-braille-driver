# Linux + wiringPi

Build library & examples using : 

```bash
cd raspberry/linux-wiringpi
./run.sh
```

Library will be generated under `out/libmetecdriver.so`.
Examples will be generated under `out/examples`.

You will have to copy the output files above on the Raspberry PI manually or write a script to do this (depending if you are using SSH, NFS...)

## Write to multiple Braille cells

```cpp
#include "braille.h"
#include "wiringPi.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         3
#define DIN        7
#define STROBE     0
#define CLK        2
#define DOUT       1

#define PATTERN_LENGTH 14
uint16_t pattern[PATTERN_LENGTH] = {
    0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
//    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
//    W         E        L        C        O        M        E             H       O        M       E             !
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void setup() {
    wiringPiSetup();
    metecDriver.init();
    metecDriver.writeCells(pattern, PATTERN_LENGTH, false);
}

int main() {
    setup();
    return 0;
}
```

## Write to single Braille cell

```cpp
#include "braille.h"
#include "wiringPi.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         3
#define DIN        7
#define STROBE     0
#define CLK        2
#define DOUT       1

#define PATTERN_LENGTH 11
uint16_t pattern[PATTERN_LENGTH] = {
    0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
//     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
//     H       E      L       L        O             W       O       R      L       D
};
MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void setup() {
    wiringPiSetup();
    metecDriver.init();
    for (uint8_t i = 0 ; i  < PATTERN_LENGTH; i++) {
        metecDriver.writeSingleCell(CELL_COUNT - 1 - i, pattern[i]);
    }
}

int main() {
    setup();
    return 0;
}
```

## Get button events

```cpp
#include "braille.h"
#include "wiringPi.h"
#include "stdio.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         3
#define DIN        7
#define STROBE     0
#define CLK        2
#define DOUT       1

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void setup() {
    wiringPiSetup();
    metecDriver.init();
}

void loop() {
    metecDriver.checkButton();
    if (metecDriver.btn_update) {
        printf("button %d switched to %d\n", metecDriver.btn_position, metecDriver.btn_state);
        metecDriver.btn_update = false;
    }
    delay(50);
}

int main() {
    setup();
    while (1)
        loop();
    return 0;
}
```

## Raspberry PI linux development environment

The build environment is using [cmake](https://cmake.org/) (see [this](https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187)). The base environment is located in the submodule [raspberry-dev](https://github.com/bertrandmartel/raspberry-dev) which download the toolchain and setup scripts to build wiringpi using cmake

Start with cloning the repo :
```bash
cd project_dir
git submodule add git://github.com/bertrandmartel/metec-braille-driver.git
git submodule update --init --recursive
```

You will need to build you own `CmakeLists.txt` like [this one](./raspberry/linux-wiringpi/CMakeLists.txt), something like :

```
cmake_minimum_required(VERSION 3.0)
project(metecdriver LANGUAGES CXX VERSION 1.0)
set (CMAKE_CXX_STANDARD 11)
include(GNUInstallDirs)
add_library(metecdriver SHARED ./metec-braille-driver/src/braille.cpp ./metec-braille-driver/src/pch.cpp)
set_target_properties(metecdriver PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    LIBRARY_OUTPUT_DIRECTORY out
    PUBLIC_HEADER ./metec-braille-driver/include/braille.h)
target_include_directories(braille PRIVATE ./metec-braille-driver/include)
set (WPI_PATH ./metec-braille-driver/raspberry/linux-wiringpi/raspberry-dev/wiringPi/wiringPi)
include_directories (include ${WPI_PATH})

#link wiringpi
find_library(WPI_LIB wiringPi HINTS ${WPI_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT WPI_LIB)
message(FATAL_ERROR "wiringPi library not found")
endif()
```
Add to this `CMakeLists.txt` your project files to be built

Then you can build this using this script : 

```bash
cmake . -DCMAKE_TOOLCHAIN_FILE=./metec-braille-driver/raspberry/linux-wiringpi/raspberry-dev/toolchain.cmake
make
```

## Building liblouis

If you want to link against liblouis, you can add to your `CMakeLists.txt` :

```
#link liblouis
set (LIBLOUIS_PATH ../liblouis/liblouis/.libs)
find_library(LOUIS_LIB louis HINTS ${LIBLOUIS_PATH} NO_CMAKE_FIND_ROOT_PATH)
if(NOT LOUIS_LIB)
message(FATAL_ERROR "liblouis library not found")
endif()
```

And to cross compile liblouis:

```bash
# cross compile liblouis
export PATH=$PATH:$PWD/metec-braille-driver/raspberry/linux-wiringpi/raspberry-dev/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin

cd /metec-braille-driver/raspberry/liblouis
if [ ! -f configure ]; then
	./autogen.sh
fi
if [ ! -f Makefile ]; then
	./configure --host arm-linux-gnueabihf
fi
make
```

Note that liblouis will generate output library under `metec-braille-driver/raspberry/liblouis/liblouis/.libs` which you will need to copy to Raspberry PI. Also you need to copy the liblouis `table` directory under `/usr/local/share/liblouis/tables` in Raspberry PI check [liblouis_install.sh](./raspberry/linux-wiringpi/liblouis_install.sh) script to install tables.

Maybe there is a better way to install liblouis but we can't just copy the liblouis directory and `sudo make install` on the Raspberry PI since some path are hardcoded in the `configure` step

## Using liblouis with libmetecdriver

```cpp
#include "braille.h"
#include "wiringPi.h"
#include <string>

#include "liblouis.h"
#include "internal.h"

using namespace MetecDriverLibrary;
using namespace std;

#define CELL_COUNT 20
#define ON         3
#define DIN        7
#define STROBE     0
#define CLK        2
#define DOUT       1

#define BRAILLE_TABLE "unicode.dis,en-gb-g1.utb"

int translateToBraille(string input, uint16_t *buffer, int* translen) {
    uint16_t inbuf[MAXSTRING];
    int inlen = _lou_extParseChars(input.c_str(), inbuf);
    return lou_translateString(BRAILLE_TABLE, inbuf, &inlen, buffer, translen, NULL, NULL, 0);
}

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

int main() {
    wiringPiSetup();
    metecDriver.init();

    string input = "hello world !";

    uint16_t buffer[MAXSTRING];
    int translen = MAXSTRING;

    if (!translateToBraille(input, buffer, &translen)) {
        printf("failed to translate input to braille\n");
        return;
    }
    metecDriver.writeCells(buffer, CELL_COUNT, false);
    lou_free();
    return 0;
}
```

# License

The MIT License (MIT) Copyright (c) 2018 Bertrand Martel