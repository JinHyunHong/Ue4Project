// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ue4Project : ModuleRules
{
	public Ue4Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ClothingSystemRuntimeInterface", "UMG", "SlateCore", "AIModule", "Niagara" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.Add(ModuleDirectory);
	}
}
