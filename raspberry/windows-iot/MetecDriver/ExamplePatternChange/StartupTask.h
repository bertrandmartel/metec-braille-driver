#pragma once

#include "pch.h"

namespace ExamplePatternChange
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class StartupTask sealed : public Windows::ApplicationModel::Background::IBackgroundTask
    {
    public:
        virtual void Run(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ taskInstance);
		
		void OnCanceled(Windows::ApplicationModel::Background::IBackgroundTaskInstance^ sender, Windows::ApplicationModel::Background::BackgroundTaskCancellationReason reason);
	private:
		Platform::Agile<Windows::ApplicationModel::Background::BackgroundTaskDeferral^> Deferral;
		Windows::ApplicationModel::Background::IBackgroundTaskInstance^ TaskInstance;
		Windows::System::Threading::ThreadPoolTimer ^Timer;
		uint8_t count = 0;
		Windows::ApplicationModel::Background::BackgroundTaskCancellationReason _cancelReason = Windows::ApplicationModel::Background::BackgroundTaskCancellationReason::Abort;
		volatile bool _cancelRequested = false;
    };
}
