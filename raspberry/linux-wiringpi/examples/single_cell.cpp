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