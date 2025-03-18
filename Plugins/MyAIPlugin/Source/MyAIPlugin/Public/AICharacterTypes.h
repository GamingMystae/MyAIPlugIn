#pragma once

#include "CoreMinimal.h"

/**
 * AI State Machine: Core behavior states for AI-controlled characters
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAIS_None       UMETA(DisplayName = "None"),
	EAIS_Idle       UMETA(DisplayName = "Idle"),
	EAIS_Patrolling UMETA(DisplayName = "Patrolling"),
	EAIS_Chasing    UMETA(DisplayName = "Chasing"),
	EAIS_Attacking  UMETA(DisplayName = "Attacking"),
	EAIS_Fleeing    UMETA(DisplayName = "Fleeing")
};

/**
 * Patrol Mode: Defines how AI selects its patrol path
 */
UENUM(BlueprintType)
enum class EPatrolMode : uint8
{
	EPM_Random    UMETA(DisplayName = "Random"),
	EPM_Waypoints UMETA(DisplayName = "Waypoints"),
	EPM_Spline    UMETA(DisplayName = "Spline Path")
};

/**
 * Combat Style: Determines AI aggression and response to threats
 */
UENUM(BlueprintType)
enum class EAICombatStyle : uint8
{
	EAIC_Passive    UMETA(DisplayName = "Passive"),
	EAIC_Aggressive UMETA(DisplayName = "Aggressive"),
	EAIC_Defensive  UMETA(DisplayName = "Defensive")
};
