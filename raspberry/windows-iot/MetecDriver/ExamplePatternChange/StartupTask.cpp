#include "StartupTask.h"
#include "braille.h"
#define CELL_COUNT 20
#define ON         22
#define DIN        4
#define STROBE     17
#define CLK        27
#define DOUT       18

using namespace MetecDriverLibrary;
using namespace ExamplePatternChange;
using namespace Windows::ApplicationModel::Background;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace concurrency;

#define PATTERN_LENGTH1 11
double pattern_tmp1[PATTERN_LENGTH1] = {
	0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
	//     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
	//     H       E      L       L        O             W       O       R      L       D
};
Array<double> ^pattern1 = ref new Array<double>(pattern_tmp1, PATTERN_LENGTH1);

#define PATTERN_LENGTH2 14
double pattern_tmp2[PATTERN_LENGTH2] = {
	0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
	//    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
	//    W         E        L        C        O        M        E             H       O        M       E             !
};
Array<double> ^pattern2 = ref new Array<double>(pattern_tmp2, PATTERN_LENGTH2);

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
	taskInstance->Canceled += ref new BackgroundTaskCanceledEventHandler(this, &StartupTask::OnCanceled);
	Deferral = taskInstance->GetDeferral();

	metecDriver.init();

	TimerElapsedHandler ^handler = ref new TimerElapsedHandler(
		[this](ThreadPoolTimer ^timer)
	{
		BackgroundTaskDeferral^ deferral = Deferral.Get();
		if (_cancelRequested == false) {
			if (count % 2 == 0) {
				metecDriver.writeCells(pattern1, PATTERN_LENGTH1, false);
			}
			else {
				metecDriver.writeCells(pattern2, PATTERN_LENGTH2, false);
			}
			count++;
			if (count == 2) {
				count = 0;
			}
		}
		else {
			timer->Cancel();
			deferral->Complete();
		}
	});

	TimeSpan interval;
	interval.Duration = 2 * 10000000; //every 2 seconds
	Timer = ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler(handler), interval);
}

void StartupTask::OnCanceled(IBackgroundTaskInstance^ sender, BackgroundTaskCancellationReason reason) {
	_cancelRequested = true;
	_cancelReason = reason;
}