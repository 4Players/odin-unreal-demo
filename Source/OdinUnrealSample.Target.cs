// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OdinUnrealSampleTarget : TargetRules
{
	public OdinUnrealSampleTarget(TargetInfo Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        Type = TargetType.Game;
		ExtraModuleNames.Add("OdinUnrealSample");
    }
}