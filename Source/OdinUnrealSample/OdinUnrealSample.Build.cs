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
		bFasterWithoutUnity = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

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

		LoadPhoton(Target);
	}

	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	private string PhotonPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "Photon")); }
	}

	private void AddPhotonLibPathWin(ReadOnlyTargetRules Target, string name)
	{
		string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Windows", name + "-cpp_vc14_release_windows_md_" + PlatformString + ".lib"));
	}

	private void AddPhotonLibPathAndroid(ReadOnlyTargetRules Target, string name)
	{
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Android", "lib" + name + "-cpp-static_debug_android_armeabi_no-rtti.a"));
	}

	private void AddPhotonLibPathIOS(ReadOnlyTargetRules Target, string name)
	{
		string archStr = (Target.Architecture == "-simulator") ? "iphonesimulator" : "iphoneos";
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "iOS", "lib" + name + "-cpp_debug_" + archStr + ".a"));
	}

	private void AddPhotonLibPathMac(ReadOnlyTargetRules Target, string name)
	{
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Mac", "lib" + name + "-cpp_debug_macosx.a"));
	}

	public bool LoadPhoton(ReadOnlyTargetRules Target)
	{
		if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
		{
			Definitions.Add("_EG_WINDOWS_PLATFORM");
			AddPhotonLibPathWin(Target, "Common");
			AddPhotonLibPathWin(Target, "Photon");
			AddPhotonLibPathWin(Target, "LoadBalancing");
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			Definitions.Add("_EG_ANDROID_PLATFORM");
			AddPhotonLibPathAndroid(Target, "common");
			AddPhotonLibPathAndroid(Target, "photon");
			AddPhotonLibPathAndroid(Target, "loadbalancing");
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			Definitions.Add("_EG_IPHONE_PLATFORM");
			AddPhotonLibPathIOS(Target, "Common");
			AddPhotonLibPathIOS(Target, "Photon");
			AddPhotonLibPathIOS(Target, "LoadBalancing");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			Definitions.Add("_EG_IMAC_PLATFORM");
			AddPhotonLibPathMac(Target, "Common");
			AddPhotonLibPathMac(Target, "Photon");
			AddPhotonLibPathMac(Target, "LoadBalancing");
		}
		else
		{
			throw new Exception("\nTarget platform not supported: " + Target.Platform);
		}

		// Include path
		PublicIncludePaths.Add(Path.Combine(PhotonPath, "."));

		return true;
	}
}
