// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class OdinUnrealSampleEditorTarget : TargetRules
{
	public OdinUnrealSampleEditorTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Editor;
		ExtraModuleNames.Add("OdinUnrealSample");
	}
}
