// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class The_Turnatable : ModuleRules
{
	public The_Turnatable(ReadOnlyTargetRules Target) : base(Target)
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
			"GameplayTags",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EventRouter",
			"UIUtils",
			"Inventory",
			"InteractionFramework",
			"InspectionFramework",
		});

		PublicIncludePaths.AddRange(new string[] {
			"The_Turnatable",
			"The_Turnatable/Variant_Horror",
			"The_Turnatable/Variant_Horror/UI",
			"The_Turnatable/Variant_Shooter",
			"The_Turnatable/Variant_Shooter/AI",
			"The_Turnatable/Variant_Shooter/UI",
			"The_Turnatable/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
