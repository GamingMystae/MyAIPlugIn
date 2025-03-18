// Fill out your copyright notice in the Description page of Project Settings.

/*****************************
** MY CUSTOM AI CONTOLER CPP *
*****************************/

#include "MyAIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "MyAIPluginSettings.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "PatrolPath.h"
#include "NPC_Character.h"
#include "Components/SplineComponent.h"
#include "SplinePath.h"

AMyAIController::AMyAIController()
{
    UE_LOG(LogTemp, Warning, TEXT("MyAIController Created!"));

    CurrentAIState = EAIState::EAIS_Idle;
    CurrentPatrolState = EPatrolMode::EPM_Random;
    CurrentCombatState = EAICombatStyle::EAIC_Passive;
    bCanAttack = true;
    bWasChasing = false;
    bReverseSpline = false;
    bInitialWaitDone = false;
    CurrentPatrolIndex = 0;
    CurrentSplineIndex = 0;
}

FString AMyAIController::GetCurrentAIStateAsString() const
{
    return UEnum::GetValueAsString(CurrentAIState);
}

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
    ANPC_Character* NPC = Cast<ANPC_Character>(MyCharacter);

    if (Settings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyAIController Initialized!"));
    }

    if (MyCharacter && Settings->bEnablePatrol)
    {
        if (Settings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("AI Character Controlled by MyAIController: %s"), *MyCharacter->GetName());
        }

        if (NPC)
        {
            PatrolPath = NPC->PatrolPath;
            SplinePath = NPC->SplinePath;

            if (Settings->PatrolMode == EPatrolMode::EPM_Waypoints && PatrolPath)
            {
                InitializePatrolPath();
            }

            if (Settings->PatrolMode == EPatrolMode::EPM_Spline && SplinePath)
            {
                UE_LOG(LogTemp, Warning, TEXT("Spline Path Set: %s"), *SplinePath->GetName());
            }
        }

        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = Settings->PatrolMoveSpeed;

        GetWorldTimerManager().SetTimer(
            PatrolTimerHandle,
            this,
            &AMyAIController::TickBehavior,
            Settings->PatrolInterval,
            true
        );
    }
}

void AMyAIController::RefreshPatrolMode()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!Settings || !GetPawn()) return;

    StopMovement();
    GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
    GetWorldTimerManager().ClearTimer(WaypointWaitTimerHandle);
    GetWorldTimerManager().ClearTimer(SplineWaitTimerHandle);

    CurrentPatrolIndex = 0;
    CurrentSplineIndex = 0;
    bWasChasing = false;
    bReverseSpline = false;

    ANPC_Character* NPC = Cast<ANPC_Character>(GetPawn());
    if (NPC)
    {
        PatrolPath = NPC->PatrolPath;
        SplinePath = NPC->SplinePath;
    }

    if (Settings->PatrolMode == EPatrolMode::EPM_Waypoints && PatrolPath)
    {
        InitializePatrolPath();
    }

    ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = Settings->PatrolMoveSpeed;
    }

    TickBehavior();
}

void AMyAIController::InitializePatrolPath()
{
    if (!PatrolPath) {
        UE_LOG(LogTemp, Warning, TEXT("[AI] InitializePatrolPath failed — PatrolPath is NULL"));
        return;
    }

    PatrolPoints = PatrolPath->PatrolPoints;

    if (const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
        Settings && Settings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Patrol Path Initialized with %d points from: %s"),
            PatrolPoints.Num(), *PatrolPath->GetName());
    }
}

void AMyAIController::TickBehavior()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!Settings || !GetPawn()) return;

    if (HandleChaseBehavior()) return;

    if (bWasChasing)
    {
        bWasChasing = false;

        if (Settings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("[AI] Player lost. Returning to patrol."));
        }
    }

    HandlePatrolBehavior();
}

void AMyAIController::MoveToNextPatrolPoint()
{
    const UMyAIPluginSettings* Settings = GetDefault<UMyAIPluginSettings>();
    if (PatrolPoints.Num() == 0) return;

    AActor* Target = PatrolPoints[CurrentPatrolIndex];
    if (!Target) return;

    MoveToActor(Target);

    float WaitTime = Settings->PatrolInterval;

    if (PatrolPath && PatrolPath->PatrolWaitTimes.IsValidIndex(CurrentPatrolIndex))
    {
        float CustomWait = PatrolPath->PatrolWaitTimes[CurrentPatrolIndex];
        if (CustomWait > 0.f) WaitTime = CustomWait;
    }

    if (Settings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Moving to Patrol Point %d: %s (Wait: %.1fs)"),
            CurrentPatrolIndex, *Target->GetName(), WaitTime);
    }

    GetWorldTimerManager().SetTimer(
        WaypointWaitTimerHandle,
        this,
        &AMyAIController::AdvancePatrolIndex,
        WaitTime,
        false
    );
}

void AMyAIController::MoveToNextSplinePoint()
{
    if (!SplinePath || !SplinePath->SplineComponent) return;

    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    USplineComponent* Spline = SplinePath->SplineComponent;

    int32 NumPoints = Spline->GetNumberOfSplinePoints();
    if (NumPoints == 0) return;

    FVector NextPoint = Spline->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
    MoveToLocation(NextPoint);

    float WaitTime = Settings->SplineWaitTime;
    if (SplinePath->WaitTimes.IsValidIndex(CurrentSplineIndex))
    {
        WaitTime = SplinePath->WaitTimes[CurrentSplineIndex];
    }

    if (Settings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Moving to spline point %d: %s (Wait: %.1fs)"),
            CurrentSplineIndex, *NextPoint.ToString(), WaitTime);
    }

    GetWorldTimerManager().SetTimer(
        SplineWaitTimerHandle,
        this,
        &AMyAIController::AdvanceSplineIndex,
        WaitTime,
        false
    );
}

void AMyAIController::AdvanceSplineIndex()
{
    if (!SplinePath || !SplinePath->SplineComponent) return;

    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    USplineComponent* Spline = SplinePath->SplineComponent;
    int32 NumPoints = Spline->GetNumberOfSplinePoints();
    if (NumPoints < 2) return;

    if (Settings->bUsePingPongSpline)
    {
        bReverseSpline ? CurrentSplineIndex-- : CurrentSplineIndex++;
        if (CurrentSplineIndex >= NumPoints)
        {
            CurrentSplineIndex = NumPoints - 2;
            bReverseSpline = true;
        }
        else if (CurrentSplineIndex < 0)
        {
            CurrentSplineIndex = 1;
            bReverseSpline = false;
        }
    }
    else
    {
        CurrentSplineIndex = (CurrentSplineIndex + 1) % NumPoints;
    }

    MoveToNextSplinePoint();
}

void AMyAIController::AdvancePatrolIndex()
{
    if (PatrolPoints.Num() == 0) return;

    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
    MoveToNextPatrolPoint();
}

bool AMyAIController::HandleChaseBehavior()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    APawn* ControlledPawn = GetPawn();
    if (!Settings || !PlayerPawn || !ControlledPawn) return false;
    if (Settings->DefaultCombatStyle == EAICombatStyle::EAIC_Passive) return false;

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), ControlledPawn->GetActorLocation());

    if (Distance <= Settings->ChaseDistance)
    {
        if (CurrentAIState != EAIState::EAIS_Chasing)
        {
            CurrentAIState = EAIState::EAIS_Chasing;
            bWasChasing = true;

            if (Settings->bEnableDebugLogs)
            {
                UE_LOG(LogTemp, Warning, TEXT("[AI] State: Chasing Player | Distance: %.2f"), Distance);
            }
        }

        MoveToActor(PlayerPawn);
        AttemptAttack();
        return true;
    }

    return false;
}

void AMyAIController::HandlePatrolBehavior()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!Settings->bEnablePatrol) return;

    CurrentAIState = EAIState::EAIS_Patrolling;

    switch (Settings->PatrolMode)
    {
    case EPatrolMode::EPM_Waypoints:
        HandleWaypointPatrol();
        break;
    case EPatrolMode::EPM_Spline:
        HandleSplinePatrol();
        break;
    case EPatrolMode::EPM_Random:
    default:
        HandleRandomPatrol();
        break;
    }
}

void AMyAIController::HandleWaypointPatrol()
{
    if (PatrolPoints.Num() == 0) return;

    if (!GetWorldTimerManager().IsTimerActive(WaypointWaitTimerHandle))
    {
        MoveToNextPatrolPoint();
    }
}

void AMyAIController::HandleSplinePatrol()
{
    if (!GetWorldTimerManager().IsTimerActive(SplineWaitTimerHandle))
    {
        MoveToNextSplinePoint();
    }
}

void AMyAIController::HandleRandomPatrol()
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;

    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    FVector Destination;
    if (NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), Destination, Settings->PatrolRadius))
    {
        if (Settings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("[AI] Patrolling to random location: %s"), *Destination.ToString());
        }

        MoveToLocation(Destination);
    }
}

void AMyAIController::AttemptAttack()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!Settings || !Settings->bEnableAttack || !bCanAttack || !PlayerPawn) return;

    if (Settings->DefaultCombatStyle == EAICombatStyle::EAIC_Passive) return;
    if (Settings->DefaultCombatStyle == EAICombatStyle::EAIC_Defensive && !bWasChasing) return;

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());

    if (Distance <= Settings->AttackRange)
    {
        CurrentAIState = EAIState::EAIS_Attacking;

        if (Settings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("Attacking Player! Distance: %f"), Distance);
        }

        bCanAttack = false;
        GetWorldTimerManager().SetTimer(
            AttackTimerHandle,
            [this]() { bCanAttack = true; },
            Settings->AttackCooldown,
            false
        );

        DrawDebugSphere(
            GetWorld(),
            GetPawn()->GetActorLocation(),
            Settings->AttackRange,
            12,
            FColor::Red,
            false,
            1.0f);
    }
}
