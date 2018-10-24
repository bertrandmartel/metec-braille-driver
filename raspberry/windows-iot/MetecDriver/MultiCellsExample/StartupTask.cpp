#include "pch.h" 
#define CELL_COUNT 20
#define ON         22
#define DIN        4
#define STROBE     17
#define CLK        27
#define DOUT       18

using namespace MetecDriverLibrary;
using namespace MultiCellsExample;
using namespace Platform;
using namespace Windows::ApplicationModel::Background;

#define PATTERN_LENGTH 14
double pattern_tmp[PATTERN_LENGTH] = {
	0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
	//    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
	//    W         E        L        C        O        M        E             H       O        M       E             !
};
Array<double> ^pattern = ref new Array<double>(pattern_tmp, PATTERN_LENGTH);

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	metecDriver.init();
	metecDriver.writeCells(pattern, PATTERN_LENGTH, false);
}