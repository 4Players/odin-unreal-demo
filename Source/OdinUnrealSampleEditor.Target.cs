// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OdinUnrealSampleEditorTarget : TargetRules
{
	public OdinUnrealSampleEditorTarget(TargetInfo Target) : base(Target)
    {
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        bOverrideBuildEnvironment = true;
        Type = TargetType.Editor;
        ExtraModuleNames.Add("OdinUnrealSample");
	}
}
