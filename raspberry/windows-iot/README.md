# Windows IoT Core

MetecDriver library is build using C++/CX as a Windows Runtime Component. It can be used from other UWP app (C++/C#/.NET).

## C++ : Single cell 

```cpp
#include "pch.h"
#define CELL_COUNT 20
#define ON         22
#define DIN        4
#define STROBE     17
#define CLK        27
#define DOUT       18

using namespace MetecDriverLibrary;
using namespace SingleCellExample;
using namespace Windows::ApplicationModel::Background;

#define PATTERN_LENGTH 11
uint16_t pattern[PATTERN_LENGTH] = {
    0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819
    //     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
    //     H       E      L       L        O             W       O       R      L       D
};

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
    metecDriver.init();
    for (uint8_t i = 0; i < PATTERN_LENGTH; i++) {
        metecDriver.writeSingleCell(CELL_COUNT - 1 - i, pattern[i]);
    }
}
```

## C++ : Multi cell 

```cpp
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
uint16_t pattern_tmp[PATTERN_LENGTH] = {
    0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816
    //    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
    //    W         E        L        C        O        M        E             H       O        M       E             !
};
Array<uint16_t> ^pattern = ref new Array<uint16_t>(pattern_tmp, PATTERN_LENGTH);

MetecDriver metecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

void StartupTask::Run(IBackgroundTaskInstance^ taskInstance)
{
    metecDriver.init();
    metecDriver.writeCells(pattern, PATTERN_LENGTH, false);
}
```

## C# : Track buttons state

```csharp
using System;
using Windows.ApplicationModel.Background;
using Windows.System.Threading;
using System.Diagnostics;

using MetecDriverLibrary;

namespace ExampleButtonSimple
{

    public sealed class StartupTask : IBackgroundTask
    {
        const byte CELL_COUNT = 20;
        const byte ON = 22;
        const byte DIN = 4;
        const byte STROBE = 17;
        const byte CLK = 27;
        const byte DOUT = 18;
  
        BackgroundTaskDeferral deferral;
        MetecDriver metecDriver = new MetecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);
        
        private ThreadPoolTimer timer;
        IBackgroundTaskInstance _taskInstance = null;

        BackgroundTaskCancellationReason _cancelReason = BackgroundTaskCancellationReason.Abort;
        volatile bool _cancelRequested = false;

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            Debug.WriteLine("Background " + taskInstance.Task.Name + " Starting...");
            taskInstance.Canceled += new BackgroundTaskCanceledEventHandler(OnCanceled);

            deferral = taskInstance.GetDeferral();
            _taskInstance = taskInstance;

            metecDriver.init();
            timer = ThreadPoolTimer.CreatePeriodicTimer(Timer_Tick, TimeSpan.FromMilliseconds(50));

        }

        private void Timer_Tick(ThreadPoolTimer timer)
        {
            if (_cancelRequested == false)
            {
                metecDriver.checkButton();
                if (metecDriver.btn_update)
                {
                    Debug.WriteLine("button " + metecDriver.btn_position + " switched to " + metecDriver.btn_state);
                    metecDriver.btn_update = false;
                }
            }
            else
            {
                timer.Cancel();
                deferral.Complete();
            }
        }

        private void OnCanceled(IBackgroundTaskInstance sender, BackgroundTaskCancellationReason reason)
        {
            _cancelRequested = true;
            _cancelReason = reason;
            Debug.WriteLine("Background " + sender.Task.Name + " Cancel Requested...");
        }
    }
}
```

## C# Multi cell

```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Http;
using Windows.ApplicationModel.Background;

using MetecDriverLibrary;

namespace ExampleMultiCell
{
    public sealed class StartupTask : IBackgroundTask
    {
        const byte CELL_COUNT = 20;
        const byte ON = 22;
        const byte DIN = 4;
        const byte STROBE = 17;
        const byte CLK = 27;
        const byte DOUT = 18;

        MetecDriver metecDriver = new MetecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

        ushort[] pattern = new ushort[] {
             0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816 };
        //    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
        //    W         E         L        C       O         M        E              H       O        M       E             !

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            metecDriver.init();
            metecDriver.writeCells(pattern, (byte)pattern.Length, false);
        }
    }
}

```

# License

The MIT License (MIT) Copyright (c) 2018 Bertrand Martel