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

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    CachedSettings = UMyAIPluginSettings::GetMutable();
    if (!CachedSettings) return;

    PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    CurrentPatrolMode = CachedSettings->PatrolMode;

    ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
    ANPC_Character* NPC = Cast<ANPC_Character>(MyCharacter);

    if (CachedSettings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("MyAIController Initialized!"));
    }

    if (MyCharacter && CachedSettings->bEnablePatrol)
    {
        if (NPC)
        {
            PatrolPath = NPC->PatrolPath;
            SplinePath = NPC->SplinePath;
        }

        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = CachedSettings->PatrolMoveSpeed;

        RefreshPatrolMode(); 

        if (!GetWorldTimerManager().IsTimerActive(PatrolTimerHandle))
        {
            GetWorldTimerManager().SetTimer(
                PatrolTimerHandle,
                this,
                &AMyAIController::TickBehavior,
                CachedSettings->PatrolInterval,
                true
            );
        }
    }
}


void AMyAIController::TickBehavior()
{
    UE_LOG(LogTemp, Warning, TEXT("[AI] TickBehavior - PatrolMode: %s"),
        *UEnum::GetValueAsString(CurrentPatrolMode));

    if (!CachedSettings || !GetPawn()) return;

    ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = CachedSettings->PatrolMoveSpeed;
    }

    if (HandleChaseBehavior()) return;

    if (bWasChasing)
    {
        bWasChasing = false;
        UE_LOG(LogTemp, Warning, TEXT("[AI] Player lost. Returning to patrol."));
    }

    HandlePatrolBehavior();
}


void AMyAIController::HandlePatrolBehavior()
{
    if (!CachedSettings || !GetPawn()) return;

    CurrentAIState = EAIState::EAIS_Patrolling;

    switch (CurrentPatrolMode)
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


bool AMyAIController::HandleChaseBehavior()
{
   // const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    APawn* ControlledPawn = GetPawn();
    if (!CachedSettings || !PlayerPawn || !ControlledPawn) return false;
    if (CachedSettings->DefaultCombatStyle == EAICombatStyle::EAIC_Passive) return false;

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), ControlledPawn->GetActorLocation());

    if (Distance <= CachedSettings->ChaseDistance)
    {
        if (CurrentAIState != EAIState::EAIS_Chasing)
        {
            CurrentAIState = EAIState::EAIS_Chasing;
            bWasChasing = true;

            if (CachedSettings->bEnableDebugLogs)
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

void AMyAIController::AttemptAttack()
{
    if (!CanAttackPlayer() || !CachedSettings || !PlayerPawn) return;

    float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetPawn()->GetActorLocation());

    if (Distance <= CachedSettings->AttackRange)
    {
        CurrentAIState = EAIState::EAIS_Attacking;

        if (CachedSettings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("Attacking Player! Distance: %f"), Distance);
        }

        bCanAttack = false;
        GetWorldTimerManager().SetTimer(
            AttackTimerHandle,
            [this]() { bCanAttack = true; },
            CachedSettings->AttackCooldown,
            false
        );

        DrawDebugSphere(
            GetWorld(),
            GetPawn()->GetActorLocation(),
            CachedSettings->AttackRange,
            12,
            FColor::Red,
            false,
            1.0f);
    }
}

bool AMyAIController::CanAttackPlayer() const
{
    //const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!CachedSettings) return false;

    if (CachedSettings->DefaultCombatStyle == EAICombatStyle::EAIC_Passive) return false;
    if (CachedSettings->DefaultCombatStyle == EAICombatStyle::EAIC_Defensive && !bWasChasing) return false;
    return true;
}

void AMyAIController::HandleRandomPatrol()
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem) return;

    if (!CachedSettings) return;
    FVector Destination;
    if (NavSystem->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), Destination, CachedSettings->PatrolRadius))
    {
        if (CachedSettings->bEnableDebugLogs)
        {
            UE_LOG(LogTemp, Warning, TEXT("[AI] Patrolling to random location: %s"), *Destination.ToString());
        }

        //MoveToLocation(Destination);
        if (FVector::Dist(GetPawn()->GetActorLocation(), Destination) > 100.f) {
            MoveToLocation(Destination);
        }
    }
}

void AMyAIController::HandleWaypointPatrol()
{
    if (PatrolPoints.Num() == 0) return;
    
        MoveToNextPatrolPoint();
    
}

void AMyAIController::MoveToNextPatrolPoint()
{
    if (PatrolPoints.Num() == 0) return;

    AActor* Target = PatrolPoints[CurrentPatrolIndex];
    if (!Target) return;

    MoveToActor(Target);

    if (CachedSettings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Moving to Patrol Point %d: %s"),
            CurrentPatrolIndex, *Target->GetName());
    }

    // Advance index — no wait, patrol timer controls pacing
    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
}


void AMyAIController::AdvancePatrolIndex()
{
    if (PatrolPoints.Num() == 0) return;

    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
    MoveToNextPatrolPoint();
}

void AMyAIController::HandleSplinePatrol()
{
    if (!GetWorldTimerManager().IsTimerActive(SplineWaitTimerHandle))
    {
        MoveToNextSplinePoint();
    }
}

void AMyAIController::MoveToNextSplinePoint()
{
    if (!SplinePath || !SplinePath->SplineComponent) return;

    //const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    USplineComponent* Spline = SplinePath->SplineComponent;

    int32 NumPoints = Spline->GetNumberOfSplinePoints();
    if (NumPoints == 0) return;

    FVector NextPoint = Spline->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
    MoveToLocation(NextPoint);

    float WaitTime = CachedSettings->SplineWaitTime;
    if (SplinePath->WaitTimes.IsValidIndex(CurrentSplineIndex))
    {
        WaitTime = SplinePath->WaitTimes[CurrentSplineIndex];
    }

    if (CachedSettings->bEnableDebugLogs)
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

    //const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    USplineComponent* Spline = SplinePath->SplineComponent;
    int32 NumPoints = Spline->GetNumberOfSplinePoints();
    if (NumPoints < 2) return;

    if (CachedSettings->bUsePingPongSpline)
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

void AMyAIController::RefreshPatrolMode()
{
    if (!CachedSettings || !GetPawn()) return;

    StopCurrentPatrol(); // clears all timers

    // Assign mode immediately
    CurrentPatrolMode = CachedSettings->PatrolMode;

    UE_LOG(LogTemp, Warning, TEXT("[AI] Refreshed patrol mode: %s"),
        *UEnum::GetValueAsString(CurrentPatrolMode));

    // Reset state
    CurrentAIState = EAIState::EAIS_Patrolling;
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

    if (CurrentPatrolMode == EPatrolMode::EPM_Waypoints)
    {
        if (PatrolPath)
        {
            InitializePatrolPath();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[AI] No PatrolPath set for Waypoint Mode!"));
        }
    }
    else if (CurrentPatrolMode == EPatrolMode::EPM_Spline)
    {
        if (SplinePath)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spline Path Set: %s"), *SplinePath->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[AI] No SplinePath set for Spline Mode!"));
        }
    }

    ACharacter* MyCharacter = Cast<ACharacter>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->GetCharacterMovement()->MaxWalkSpeed = CachedSettings->PatrolMoveSpeed;
    }

    // Restart patrol timer
    if (!GetWorldTimerManager().IsTimerActive(PatrolTimerHandle))
    {
        GetWorldTimerManager().SetTimer(
            PatrolTimerHandle,
            this,
            &AMyAIController::TickBehavior,
            CachedSettings->PatrolInterval,
            true
        );
    }
}

void AMyAIController::StopCurrentPatrol()
{
    StopMovement();

    if (GetWorldTimerManager().IsTimerActive(PatrolTimerHandle))
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Stopping Patrol Timer."));
    }

    GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
    GetWorldTimerManager().ClearTimer(WaypointWaitTimerHandle);
    GetWorldTimerManager().ClearTimer(SplineWaitTimerHandle);
}


void AMyAIController::InitializeCurrentPatrolMode()
{
    //const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!CachedSettings || !GetPawn()) return;
    CurrentPatrolMode = CachedSettings->PatrolMode;
    switch (CurrentPatrolMode)
    {
    case EPatrolMode::EPM_Waypoints:
        if (PatrolPath) InitializePatrolPath();
        break;

    case EPatrolMode::EPM_Spline:
        if (SplinePath)
        {
            UE_LOG(LogTemp, Warning, TEXT("Spline Path Set: %s"), *SplinePath->GetName());
        }
        break;

    case EPatrolMode::EPM_Random:
    default:
        break;
    }
}

void AMyAIController::InitializePatrolPath()
{
    if (!PatrolPath) {
        UE_LOG(LogTemp, Warning, TEXT("[AI] InitializePatrolPath failed — PatrolPath is NULL"));
        return;
    }

    PatrolPoints = PatrolPath->PatrolPoints;

    if (CachedSettings && CachedSettings->bEnableDebugLogs)
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Patrol Path Initialized with %d points from: %s"),
            PatrolPoints.Num(), *PatrolPath->GetName());
    }
}

FString AMyAIController::GetCurrentAIStateAsString() const
{
    return UEnum::GetValueAsString(CurrentAIState);
}
