#pragma once
 
#include "Modules/ModuleManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemOdinFleet.h"

class FOnlineFactoryOdinFleet : public IOnlineFactory
{
public:
	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override
	{
		auto OnlineSubPtr = MakeShared<FOnlineSubsystemOdinFleet, ESPMode::ThreadSafe>(InstanceName);

		if (OnlineSubPtr->IsEnabled())
		{
			if (!OnlineSubPtr->Init())
			{
				UE_LOG(LogTemp, Error, TEXT("Could not initialize Odin Fleet Subsystem!"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Initialized Odin Fleet Subsystem!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Odin Fleet Online Subsystem was disabled!"));
			OnlineSubPtr->Shutdown();
		}

		return OnlineSubPtr;
	}
};

class FOnlineSubsystemOdinFleetModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	TUniquePtr<FOnlineFactoryOdinFleet> OdinFleetFactory;
};