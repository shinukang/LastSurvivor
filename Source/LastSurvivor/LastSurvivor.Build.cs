// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LastSurvivor : ModuleRules
{
	public LastSurvivor(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"Eigen", "Slate", "SlateCore", "GameplayCameras" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "AIModule", "GameplayTasks", "NavigationSystem"  });
	}
}
