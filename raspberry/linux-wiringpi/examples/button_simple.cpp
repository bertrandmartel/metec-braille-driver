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

button key;

void setup() {
    wiringPiSetup();
    metecDriver.init();
}

void loop() {
    metecDriver.checkButton(&key);
    if (key.update) {
        printf("button %d switched to %d\n", key.position, key.state);
        key.update = false;
    }
    delay(50);
}

int main() {
    setup();
    while (1)
        loop();
    return 0;
}