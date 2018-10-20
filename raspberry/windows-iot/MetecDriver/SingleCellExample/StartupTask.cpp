#include "pch.h"
#pragma comment(lib, "..\\ARM\\Debug\\MetecDriverLibrary\\MetecDriverLibrary.lib")
#include "braille.h" 
#define CELL_COUNT 20
#define ON         22
#define DIN        4
#define STROBE     17
#define CLK        27
#define DOUT       18

using namespace MetecDriverLibrary;
using namespace SingleCellExample;
using namespace Windows::ApplicationModel::Background;

#define PATTERN_LENGTH1 11
uint16_t pattern1[PATTERN_LENGTH1] = {
	0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
	//     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
	//     H       E      L       L        O             W       O       R      L       D
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	metecDriver.init();
	metecDriver.writeCells(pattern1, PATTERN_LENGTH1, false);
}