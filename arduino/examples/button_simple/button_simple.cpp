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