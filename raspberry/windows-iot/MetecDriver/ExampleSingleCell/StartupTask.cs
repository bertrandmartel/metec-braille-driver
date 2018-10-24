using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Http;
using Windows.ApplicationModel.Background;

using MetecDriverLibrary;

namespace ExampleSingleCell
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
            0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819 };
        //     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
        //     H       E       L       L        O             W       O       R       L       D

        public void Run(IBackgroundTaskInstance taskInstance)
        {
            metecDriver.init();
            for (int i = 0; i < pattern.Length; i++)
            {
                metecDriver.writeSingleCell((byte)(CELL_COUNT - 1 - i), pattern[i]);
            }
        }
    }
}
