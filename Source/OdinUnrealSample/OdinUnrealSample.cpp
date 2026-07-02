#include "OdinUnrealSample.h"

#include "Misc/ConfigCacheIni.h"
#include "Misc/Crc.h"
#include "Misc/EngineVersion.h"
#include "Misc/NetworkVersion.h"
#include "Modules/ModuleManager.h"

namespace
{
	uint32 GetLocalNetworkVersionFromEngineAndGameVersion()
	{
		const FString EngineVersion = FEngineVersion::Current().ToString(EVersionComponent::Patch);
		FString GameVersion = FNetworkVersion::GetProjectVersion();

		if (GConfig)
		{
			GConfig->GetString(
				TEXT("/Script/EngineSettings.GeneralProjectSettings"),
				TEXT("ProjectVersion"),
				GameVersion,
				GGameIni);
		}

		if (GameVersion.IsEmpty())
		{
			GameVersion = FNetworkVersion::GetProjectVersion();
		}

		const FString VersionString = FString::Printf(TEXT("Engine=%s;Game=%s"), *EngineVersion, *GameVersion);
		return FCrc::StrCrc32(*VersionString.ToLower());
	}
}

class FOdinUnrealSampleModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FString GameVersion = FNetworkVersion::GetProjectVersion();
		if (GConfig)
		{
			GConfig->GetString(
				TEXT("/Script/EngineSettings.GeneralProjectSettings"),
				TEXT("ProjectVersion"),
				GameVersion,
				GGameIni);
		}

		if (!GameVersion.IsEmpty())
		{
			FNetworkVersion::SetProjectVersion(*GameVersion);
		}

		FNetworkVersion::GetLocalNetworkVersionOverride.BindStatic(&GetLocalNetworkVersionFromEngineAndGameVersion);
	}

	virtual void ShutdownModule() override
	{
		if (FNetworkVersion::GetLocalNetworkVersionOverride.IsBound())
		{
			FNetworkVersion::GetLocalNetworkVersionOverride.Unbind();
		}
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FOdinUnrealSampleModule, OdinUnrealSample, "OdinUnrealSample");
