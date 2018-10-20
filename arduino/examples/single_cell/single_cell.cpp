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