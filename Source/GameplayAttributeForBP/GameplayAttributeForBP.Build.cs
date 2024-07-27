// Copyright 2023-2024 Metaseven. All Rights Reserved.

using UnrealBuildTool;

public class GameplayAttributeForBP : ModuleRules
{
	public GameplayAttributeForBP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"UnrealEd",
				"Blutility",
				"ToolMenus",
				"GameplayAbilities",
			}
		);
	}
}
