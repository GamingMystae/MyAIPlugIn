// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugUIManager.h"
#include "DebugAISettingsWidget.h"
#include "MyAIPluginSettings.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "NPC_Character.h"
#include "MyAIController.h"

ADebugUIManager::ADebugUIManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ADebugUIManager::BeginPlay()
{
    Super::BeginPlay();

    RuntimeSettings = UMyAIPluginSettings::GetMutable();
   

    if (!DebugWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("DebugWidgetClass is not assigned! Please set it in the Blueprint."));
        return;
    }

    APlayerController* PC = GetLocalPlayerController();
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController in DebugUIManager."));
        return;
    }

    // Create and assign the debug widget
    DebugWidget = CreateWidget<UDebugAISettingsWidget>(PC, DebugWidgetClass);
    if (!DebugWidget)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create DebugWidget from class %s"), *DebugWidgetClass->GetName());
        return;
    }

    DebugWidget->DebugUIManagerRef = this;

    // Try to auto-assign an AIController for testing
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC_Character::StaticClass(), FoundActors);

    UE_LOG(LogTemp, Warning, TEXT("Found %d NPC_Characters in the world."), FoundActors.Num());

    if (FoundActors.Num() > 0)
    {
        ANPC_Character* NPC = Cast<ANPC_Character>(FoundActors[0]);
        if (NPC)
        {
            AMyAIController* AIController = Cast<AMyAIController>(NPC->GetController());
            if (AIController)
            {
                DebugWidget->DebugAIControllerRef = AIController;
                UE_LOG(LogTemp, Warning, TEXT("Assigned DebugAIControllerRef: %s"), *AIController->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("NPC found but has no valid AMyAIController."));
            }
        }
    }

    // Add the debug widget to the screen (initially hidden)
    DebugWidget->AddToViewport(100);
    DebugWidget->SetVisibility(ESlateVisibility::Hidden);

    // Bind key for toggling
    EnableInput(PC);
    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::T, IE_Pressed, this, &ADebugUIManager::ToggleDebugUI);
    }
}

void ADebugUIManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADebugUIManager::ToggleDebugUI()
{
    if (!DebugWidgetClass || !DebugWidget) return;

    APlayerController* PC = GetLocalPlayerController();
    if (!PC) return;

    bIsVisible = !bIsVisible;

    DebugWidget->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

    if (bIsVisible)
    {
        DebugWidget->SetFocus();

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(DebugWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false); // optional: keep cursor
        PC->SetInputMode(InputMode);


        UE_LOG(LogTemp, Log, TEXT("Debug UI is now Visible"));
    }
    else
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;

        UE_LOG(LogTemp, Warning, TEXT("Debug UI is now Hidden"));
        UE_LOG(LogTemp, Warning, TEXT("Returned to Game Mode (Play Mode Resumed)"));
    }

    OnDebugToggled(bIsVisible);
}

/* ===== PATROL & COMBAT SETTINGS ===== */

void ADebugUIManager::SetCombatStyle(EAICombatStyle NewStyle)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->DefaultCombatStyle = NewStyle;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Combat style set to %s"),
            *UEnum::GetValueAsString(NewStyle));
    }
}

void ADebugUIManager::SetPatrolStyle(EPatrolMode NewMode)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->PatrolMode = NewMode;

        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Patrol mode set to %s"),
            *UEnum::GetValueAsString(NewMode));
    }

    //Use the stored reference to your AI controller
    if (DebugWidget && DebugWidget->DebugAIControllerRef)
    {
        DebugWidget->DebugAIControllerRef->RefreshPatrolMode();
    }
}


void ADebugUIManager::SetEnableDebugLogs(bool bEnabled)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->bEnableDebugLogs = bEnabled;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Debug logs %s"),
            bEnabled ? TEXT("ENABLED") : TEXT("DISABLED"));
    }
}

void ADebugUIManager::SetPatrolSpeed(float NewSpeed)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->PatrolMoveSpeed = NewSpeed;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Patrol Speed changed via slider: %.2f"), NewSpeed);
        RefreshAIControllerPatrol();
    }
}

void ADebugUIManager::SetPatrolInterval(float NewInterval)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->PatrolInterval = NewInterval;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Patrol Interval changed via slider: %.2f"), NewInterval);
        RefreshAIControllerPatrol();
    }
}

void ADebugUIManager::SetPatrolRadius(float NewRadius)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->PatrolRadius = NewRadius;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Patrol Radius changed via slider: %.2f"), NewRadius);
        RefreshAIControllerPatrol();
    }
}

void ADebugUIManager::SetPingPongSpline(bool bPingPong)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->bUsePingPongSpline = bPingPong;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] PingPong Spline set to: %s"), bPingPong ? TEXT("True") : TEXT("False"));
        RefreshAIControllerPatrol();
    }
}

void ADebugUIManager::SetSplineWaitTime(float NewWaitTime)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->SplineWaitTime = NewWaitTime;
        UE_LOG(LogTemp, Warning, TEXT("[DebugUIManager] Spline Wait Time changed via slider: %.2f"), NewWaitTime);
        RefreshAIControllerPatrol();
    }
}

void ADebugUIManager::SetEnableChasing(bool bEnable)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->bEnableChase = bEnable;
    }
}

void ADebugUIManager::SetChaseDistance(float Distance)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->ChaseDistance = Distance;
    }
}

void ADebugUIManager::SetEnableAttacking(bool bEnable)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->bEnableAttack = bEnable;
    }
}

void ADebugUIManager::SetAttackRange(float Range)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->AttackRange = Range;
    }
}

void ADebugUIManager::SetAttackCooldown(float Cooldown)
{
    if (RuntimeSettings)
    {
        RuntimeSettings->AttackCooldown = Cooldown;
    }
}

/* ===== HELPER ===== */

APlayerController* ADebugUIManager::GetLocalPlayerController() const
{
    return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ADebugUIManager::RefreshAIControllerPatrol()
{
    if (DebugAIControllerRef)
    {
        DebugAIControllerRef->RefreshPatrolMode();
    }
}



