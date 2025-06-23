// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

// https://wiki.unrealengine.com/Linking_Static_Libraries_Using_The_Build_System

using System;
using System.IO;
using UnrealBuildTool;

public class OdinUnrealSample : ModuleRules
{
	public OdinUnrealSample(ReadOnlyTargetRules Target) : base(Target)
	{
		MinFilesUsingPrecompiledHeaderOverride = 1;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemEOS", "ApplicationCore", "OnlineSubsystemOdinFleet" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

		DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.Add("Slate");

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}

	private string ModulePath
	{
		get { return ModuleDirectory; }
	}
}
