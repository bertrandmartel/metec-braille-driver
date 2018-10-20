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

#define BRAILLE_TABLE "unicode.dis,en-gb-g1.utb" //fr-bfu-g2.ctb

int translateToBraille(string input, uint16_t *buffer, int* translen) {
    uint16_t inbuf[MAXSTRING];
    int inlen = _lou_extParseChars(input.c_str(), inbuf);
    return lou_translateString(BRAILLE_TABLE, inbuf, &inlen, buffer, translen, NULL, NULL, 0);
}

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void setup() {
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
}

int main() {
    setup();
    return 0;
}