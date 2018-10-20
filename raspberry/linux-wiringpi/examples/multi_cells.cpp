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