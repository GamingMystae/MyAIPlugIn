// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIPlugin.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FAIPluginModule, AIPlugin, "AIPlugin");

void FAIPluginModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("AIPlugin Module Started!"));
}

void FAIPluginModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("AIPlugin Module Shut Down!"));
}
