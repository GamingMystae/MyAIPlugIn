// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AIPlugin : ModuleRules
{
	public AIPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
            "AIModule", // AI Behavior Trees, Perception
			"GameplayTasks", // Required for AI movement & tasks
			"NavigationSystem" // AI Pathfinding
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate", 
			"SlateCore", 
			"InputCore" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
