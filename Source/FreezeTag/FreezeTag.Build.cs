// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FreezeTag : ModuleRules
{
	public FreezeTag(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"SlateCore",
			"OnlineSubsystem",
			"OnlineSubsystemUtils"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FreezeTag",
			"FreezeTag/Variant_Platforming",
			"FreezeTag/Variant_Platforming/Animation",
			"FreezeTag/Variant_Combat",
			"FreezeTag/Variant_Combat/AI",
			"FreezeTag/Variant_Combat/Animation",
			"FreezeTag/Variant_Combat/Gameplay",
			"FreezeTag/Variant_Combat/Interfaces",
			"FreezeTag/Variant_Combat/UI",
			"FreezeTag/Variant_SideScrolling",
			"FreezeTag/Variant_SideScrolling/AI",
			"FreezeTag/Variant_SideScrolling/Gameplay",
			"FreezeTag/Variant_SideScrolling/Interfaces",
			"FreezeTag/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
