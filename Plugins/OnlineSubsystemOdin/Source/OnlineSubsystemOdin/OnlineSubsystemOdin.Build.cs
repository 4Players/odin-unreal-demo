// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemOdin : ModuleRules
{
	public OnlineSubsystemOdin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                "OnlineSubsystemOdin/Public"
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "OnlineSubsystemOdin/Private"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreOnline",
				"CoreUObject",
				"Engine",
				"OnlineSubsystem",
				"HTTP"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
                "CoreOnline",
                "Engine",
				"Slate",
				"SlateCore",
				"OnlineSubsystemUtils",
				"Json",
                "JsonUtilities",
                "OnlineSubsystem",
				"Sockets"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
