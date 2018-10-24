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
