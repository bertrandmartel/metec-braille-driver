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