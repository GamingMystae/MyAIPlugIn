// Fill out your copyright notice in the Description page of Project Settings.

/********************************
** MY CUSTOM AI CONTOLER HEADER *
*********************************/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIPluginSettings.h"
#include "MyAIController.generated.h"

// Forward declarations to avoid unnecessary includes
class ATargetPoint;
class APatrolPath;
class ASplinePath;
class USplineComponent;
class ANPC_Character;

/**
 * Custom AI Controller supporting Patrol (Waypoint, Spline, Random),
 * Chase and Combat Behavior, and Debug Visualization
 */
UCLASS()
class MYAIPLUGIN_API AMyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMyAIController();

    /** Returns current AI State as readable string */
    FString GetCurrentAIStateAsString() const;

    /** Called when the game starts */
    virtual void BeginPlay() override;

    /** Resets patrol mode and reloads points */
    void RefreshPatrolMode();

    /** Initializes patrol point list */
    void InitializePatrolPath();

protected:
    /** Main logic handler, runs every PatrolInterval */
    void TickBehavior();

    /** Patrol movement: Waypoints */
    void MoveToNextPatrolPoint();
    void AdvancePatrolIndex();
    void HandleWaypointPatrol();

    /** Patrol movement: Spline */
    void MoveToNextSplinePoint();
    void AdvanceSplineIndex();
    void HandleSplinePatrol();

    /** Patrol movement: Random */
    void HandleRandomPatrol();

    /** Chase and Attack Behavior */
    bool HandleChaseBehavior();
    void AttemptAttack();

    /** Main patrol switch based on current settings */
    void HandlePatrolBehavior();

private:

    /** Pointer to player pawn */
    APawn* PlayerPawn = nullptr;

    /** Patrol & Spline references set in NPC */
    APatrolPath* PatrolPath = nullptr;
    ASplinePath* SplinePath = nullptr;

    /** Current patrol state tracking */
    int32 CurrentPatrolIndex = 0;
    int32 CurrentSplineIndex = 0;

    /** AI behavior state machine */
    UPROPERTY(VisibleAnywhere)
    EAIState CurrentAIState;

    UPROPERTY(VisibleAnywhere)
    EPatrolMode CurrentPatrolState;

    UPROPERTY(VisibleAnywhere)
    EAICombatStyle CurrentCombatState;

    /** Control flags */
    bool bCanAttack;
    bool bWasChasing;
    bool bReverseSpline;
    bool bInitialWaitDone;

    /** Waypoint patrol points (loaded from APatrolPath) */
    TArray<AActor*> PatrolPoints;

    /** Timers for logic pacing */
    FTimerHandle PatrolTimerHandle;
    FTimerHandle WaypointWaitTimerHandle;
    FTimerHandle SplineWaitTimerHandle;
    FTimerHandle AttackTimerHandle;
};
