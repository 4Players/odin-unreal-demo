#pragma once
 
#include "Modules/ModuleManager.h"

class FOnlineSubsystemOdinModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	void StartupModule();
	void ShutdownModule();
};