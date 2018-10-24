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
