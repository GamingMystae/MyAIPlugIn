// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyAIPlugin.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "MyAIPluginSettings.h"

#define LOCTEXT_NAMESPACE "FMyAIPluginModule"

void FMyAIPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->RegisterSettings(
            "Project",           // Category
            "Plugins",           // Section
            "MyAIPlugin",        // Name shown in settings
            FText::FromString("My AI Plugin"),
            FText::FromString("Settings for AI behavior."),
            GetMutableDefault<UMyAIPluginSettings>()
        );
    }

	UE_LOG(LogTemp, Warning, TEXT("MyAIPlugin Started!")); // debug that the plugin is loaded and start

}

void FMyAIPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Warning, TEXT("MyAIPlugin Shut Down!")); //debug that the plugin is ended
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyAIPluginModule, MyAIPlugin)