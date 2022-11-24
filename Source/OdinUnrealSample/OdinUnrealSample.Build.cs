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
		string PlatformString = "x64";
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Windows", name + "-cpp_vc14_release_windows_md_" + PlatformString + ".lib"));
	}

	private void AddPhotonLibPathAndroid(ReadOnlyTargetRules Target, string name)
	{
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Android", "lib" + name + "-cpp-static_debug_android_armeabi_no-rtti.a"));
	}

	private void AddPhotonLibPathIOS(ReadOnlyTargetRules Target, string name)
	{
		string archStr = (Target.Architecture == "-simulator") ? "iphonesimulator" : "iphoneos";
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "iOS", "lib" + name + "_debug_" + archStr + ".a"));
	}

	private void AddPhotonLibPathMac(ReadOnlyTargetRules Target, string name)
	{
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Mac", "lib" + name + "_debug_macosx.a"));
	}

	public bool LoadPhoton(ReadOnlyTargetRules Target)
	{
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			//GlobalDefinitions.Add("_EG_WINDOWS_PLATFORM");
			PublicDefinitions.Add("_EG_WINDOWS_PLATFORM");

			AddPhotonLibPathWin(Target, "Common");
			AddPhotonLibPathWin(Target, "Photon");
			AddPhotonLibPathWin(Target, "LoadBalancing");
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			//GlobalDefinitions.Add("_EG_ANDROID_PLATFORM");

			PublicDefinitions.Add("_EG_ANDROID_PLATFORM");

			AddPhotonLibPathAndroid(Target, "common");
			AddPhotonLibPathAndroid(Target, "photon");
			AddPhotonLibPathAndroid(Target, "loadbalancing");
			AddPhotonLibPathMac(Target, "crypto");
			// TODO: Add libs for ssl and websockets to support android
			//AddPhotonLibPathMac(Target, "ssl");
			//ddPhotonLibPathMac(Target, "websockets");

		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			//GlobalDefinitions.Add("_EG_IPHONE_PLATFORM");
			PublicDefinitions.Add("_EG_IPHONE_PLATFORM");


			AddPhotonLibPathIOS(Target, "Common-cpp");
			AddPhotonLibPathIOS(Target, "Photon-cpp");
			AddPhotonLibPathIOS(Target, "LoadBalancing-cpp");
			AddPhotonLibPathMac(Target, "crypto");

		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			//GlobalDefinitions.Add("_EG_IMAC_PLATFORM");
			PublicDefinitions.Add("_EG_IMAC_PLATFORM");
			AddPhotonLibPathMac(Target, "Common-cpp");
			AddPhotonLibPathMac(Target, "Photon-cpp");
			AddPhotonLibPathMac(Target, "LoadBalancing-cpp");
			AddPhotonLibPathMac(Target, "crypto");
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
