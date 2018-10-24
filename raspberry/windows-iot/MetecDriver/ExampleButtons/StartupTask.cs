using System;
using Windows.ApplicationModel.Background;
using Windows.System.Threading;
using System.Diagnostics;

using MetecDriverLibrary;

namespace ExampleButtons
{

    public sealed class StartupTask : IBackgroundTask
    {
        const byte CELL_COUNT = 20;
        const byte ON = 22;
        const byte DIN = 4;
        const byte STROBE = 17;
        const byte CLK = 27;
        const byte DOUT = 18;

        uint count = 0;

        bool computing = false;

        BackgroundTaskDeferral deferral;
        MetecDriver metecDriver = new MetecDriver(CELL_COUNT, ON, DIN, STROBE, CLK, DOUT);

        double[] pattern1 = new double[] {
            0x2813, 0x2811, 0x2807, 0x2807, 0x2815, 0x20, 0x283a, 0x2815, 0x2817, 0x2807, 0x2819 };
        //     ⠓       ⠑      ⠇       ⠇       ⠕             ⠺       ⠕      ⠗       ⠇      ⠙
        //     H       E      L       L        O             W       O       R      L       D

        double[] pattern2 = new double[] {
             0x283a,  0x2811,  0x2807,  0x2809,  0x2815,  0x280d,  0x2811, 0x20,  0x2813, 0x2815, 0x280d, 0x2811, 0x20, 0x2816 };
        //    ⠺         ⠑        ⠇        ⠉       ⠕        ⠍        ⠑             ⠓       ⠕       ⠍       ⠑             ⠖
        //    W         E        L        C        O        M        E             H       O        M       E             !

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
                if (computing == false)
                {
                    computing = true;
                    metecDriver.checkButton();
                    if (metecDriver.btn_update)
                    {
                        Debug.WriteLine("button " + metecDriver.btn_position + " switched to " + metecDriver.btn_state);
                        metecDriver.btn_update = false;
                        if (metecDriver.btn_state == ButtonState.Pushed)
                        {
                            if (count % 2 == 0)
                            {
                                metecDriver.writeCells(pattern1, (byte)pattern1.Length, false);
                            }
                            else
                            {
                                metecDriver.writeCells(pattern2, (byte)pattern2.Length, false);
                            }
                            count++;
                            if (count == 2)
                            {
                                count = 0;
                            }
                        }
                    }
                    computing = false;
                }
                else
                {
                    Debug.WriteLine("previous check button is computing ...");
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
