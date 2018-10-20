#include "braille.h"

using namespace MetecDriverLibrary;

#define CELL_COUNT 20
#define ON         6
#define DIN        3
#define STROBE     4
#define CLK        5
#define DOUT       2

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

button key;

int main() {
    init();
    metecDriver.init();
    Serial.begin(9600);
    while (1) {
        metecDriver.checkButton(&key);
        if (key.update) {
            Serial.print("button ");
            Serial.print(key.position);
            Serial.print(" switched to ");
            Serial.println(key.state);
            key.update = false;
        }
        delay(50);
    }
    return 0;
}