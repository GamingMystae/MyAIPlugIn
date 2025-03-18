/*****************************
** Custom Settings For User  *
*****************************/
#pragma once

#include "AICharacterTypes.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyAIPluginSettings.generated.h"

/**
 * Configurable settings for MyAIPlugin (visible in Project Settings > MyAIPlugin).
 * Controls AI patrol, combat, and debug behavior.
 */
UCLASS(config = Game, defaultconfig, EditInlineNew)
class MYAIPLUGIN_API UMyAIPluginSettings : public UObject
{
    GENERATED_BODY()

public:

    /****************
    * PATROLLING AI *
    ****************/

    /** Enables patrolling behavior */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling")
    bool bEnablePatrol = true;

    /** Max random radius for EPM_Random patrol mode */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
    float PatrolRadius = 1000.0f;

    /** Interval between patrol checks or movements */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float PatrolInterval = 3.0f;

    /** Walking speed while patrolling */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling", meta = (ClampMin = "100.0", ClampMax = "1200.0"))
    float PatrolMoveSpeed = 600.0f;

    /** Patrol style (Random, Waypoints, or Spline) */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling")
    EPatrolMode PatrolMode = EPatrolMode::EPM_Random;

    /** If true, spline path patrol will ping-pong back and forth */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling|Spline", meta = (ToolTip = "True: ReverseDirection / False: Loop"))
    bool bUsePingPongSpline = false;

    /** Default wait time at each spline point (can be overridden per point) */
    UPROPERTY(EditAnywhere, Config, Category = "Patrolling|Spline", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float SplineWaitTime = 1.0f;

    /***********
    * COMBAT AI *
    ***********/

    /** Default combat style (Passive, Defensive, Aggressive) */
    UPROPERTY(EditAnywhere, Config, Category = "Combat")
    EAICombatStyle DefaultCombatStyle = EAICombatStyle::EAIC_Passive;

    /** If true, AI will chase the player */
    UPROPERTY(EditAnywhere, Config, Category = "Combat")
    bool bEnableChase = true;

    /** If true, AI will attack when close to the player */
    UPROPERTY(EditAnywhere, Config, Category = "Combat")
    bool bEnableAttack = true;

    /** Max distance to start chasing the player */
    UPROPERTY(EditAnywhere, Config, Category = "Combat", meta = (ClampMin = "100.0", ClampMax = "5000.0"))
    float ChaseDistance = 800.0f;

    /** Max range to trigger attack */
    UPROPERTY(EditAnywhere, Config, Category = "Combat", meta = (ClampMin = "50.0", ClampMax = "1000.0"))
    float AttackRange = 150.0f;

    /** Cooldown between attacks */
    UPROPERTY(EditAnywhere, Config, Category = "Combat", meta = (ClampMin = "0.1", ClampMax = "10.0"))
    float AttackCooldown = 1.5f;

    /********
    * DEBUG *
    ********/

    /** Enables log output for debug purposes */
    UPROPERTY(EditAnywhere, Config, Category = "Debug")
    bool bEnableDebugLogs = true;

public:

    /** Returns the global settings object */
    UFUNCTION(BlueprintCallable, Category = "AI Settings")
    static UMyAIPluginSettings* GetMutable();
};
