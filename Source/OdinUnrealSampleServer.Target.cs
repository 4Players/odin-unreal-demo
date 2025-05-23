// Insert Your Copyright here

using UnrealBuildTool;
using System.Collections.Generic;

public class OdinUnrealSampleServerTarget : TargetRules
{
	public OdinUnrealSampleServerTarget(TargetInfo Target) : base(Target)
	{
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        bOverrideBuildEnvironment = true;

		Type = TargetType.Server;

		ExtraModuleNames.Add("OdinUnrealSample");
	}
}