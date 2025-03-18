// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIPluginSettings.h"
#include "UObject/ConstructorHelpers.h"

UMyAIPluginSettings* UMyAIPluginSettings::GetMutable()
{
    static UMyAIPluginSettings* RuntimeInstance = nullptr;

    if (!RuntimeInstance)
    {
        RuntimeInstance = DuplicateObject<UMyAIPluginSettings>(GetMutableDefault<UMyAIPluginSettings>(), nullptr);
        RuntimeInstance->AddToRoot(); // Prevent GC

        UE_LOG(LogTemp, Log, TEXT("[MyAIPlugin] Settings initialized from config."));
    }

    return RuntimeInstance;
}
