// Fill out your copyright notice in the Description page of Project Settings.

/******************************
** MY CUSTOM AI Character CPP *
******************************/
#include "NPC_Character.h"
#include "MyAIController.h"
#include "MyAIPluginSettings.h"
#include "SplinePath.h"
#include "PatrolPath.h"

ANPC_Character::ANPC_Character()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set the default AI Controller to our custom controller
	AIControllerClass = AMyAIController::StaticClass();
}

void ANPC_Character::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("NPC_Character Spawned: %s"), *GetName());

	const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
	if (!Settings) return;

	if (Settings->PatrolMode == EPatrolMode::EPM_Spline && SplinePath)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline Path Exists on NPC: %s"), *SplinePath->GetName());
	}

	if (Settings->PatrolMode == EPatrolMode::EPM_Waypoints && PatrolPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol Path Exists on NPC: %s"), *PatrolPath->GetName());
	}
}



