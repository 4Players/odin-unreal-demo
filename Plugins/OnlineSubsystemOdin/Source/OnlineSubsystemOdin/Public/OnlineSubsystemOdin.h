#pragma once
 
#include "Modules/ModuleManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemOdinFleet.h"

class FOnlineFactoryOdinFleet : public IOnlineFactory
{
public:
	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override
	{
		return MakeShared<FOnlineSubsystemOdinFleet, ESPMode::ThreadSafe>(InstanceName);
	}
};

class FOnlineSubsystemOdinModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();

	TUniquePtr<FOnlineFactoryOdinFleet> OdinFleetFactory;
};