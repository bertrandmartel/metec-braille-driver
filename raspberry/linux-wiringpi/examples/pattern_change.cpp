#include "braille.h"
#include "wiringPi.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         3
#define DIN        7
#define STROBE     0
#define CLK        2
#define DOUT       1

#define PATTERN_LENGTH1 11
uint16_t pattern1[PATTERN_LENGTH1] = {
    0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
//     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
//     H       E      L       L        O             W       O       R      L       D
};

#define PATTERN_LENGTH2 14
uint16_t pattern2[PATTERN_LENGTH2] = {
    0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
//    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
//    W         E        L        C        O        M        E             H       O        M       E             !
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

uint8_t count = 0;

void setup() {
    wiringPiSetup();
    metecDriver.init();
}

void loop() {
    if (count % 2 == 0) {
        metecDriver.writeCells(pattern1, PATTERN_LENGTH1, false);
    } else {
        metecDriver.writeCells(pattern2, PATTERN_LENGTH2, false);
    }
    count++;
    delay(1000);
    if (count == 2) {
        count = 0;
    }
}

int main() {
    setup();
    while (1)
        loop();
    return 0;
}