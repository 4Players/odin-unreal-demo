// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemOdin.h"
#include "OnlineSubsystemModule.h"
#include "Modules/ModuleManager.h"
#include "OnlineSubsystemOdinFleet.h"

#define LOCTEXT_NAMESPACE "FOnlineSubsystemOdinModule"

void FOnlineSubsystemOdinModule::StartupModule()
{
	OdinFleetFactory = MakeUnique<FOnlineFactoryOdinFleet>();

	FOnlineSubsystemModule& OSSModule = FModuleManager::LoadModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");

	OSSModule.RegisterPlatformService(TEXT("OdinFleet"), OdinFleetFactory.Get());
}

void FOnlineSubsystemOdinModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (FModuleManager::Get().IsModuleLoaded("OnlineSubsystem"))
	{
		FOnlineSubsystemModule& OSSModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
		OSSModule.UnregisterPlatformService(TEXT("OdinFleet"));
	}

	OdinFleetFactory.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOnlineSubsystemOdinModule, OnlineSubsystemOdin)