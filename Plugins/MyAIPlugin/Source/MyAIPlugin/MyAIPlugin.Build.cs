// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyAIPlugin : ModuleRules
{
	public MyAIPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "AIModule",          // AI Behavior Trees, Perception, AIController
				"GameplayTasks",     // Required for AI movement, tasks, decision-making
				"NavigationSystem",   // AI Pathfinding (NavMesh)
				"UMG",				 // Required for using UUserWidget and UI creation
				"Slate",		 // Needed for UI elements (e.g., buttons, text)
				"SlateCore"       // Required for UI style definitions and widgets
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
                "InputCore",
                "Settings"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
