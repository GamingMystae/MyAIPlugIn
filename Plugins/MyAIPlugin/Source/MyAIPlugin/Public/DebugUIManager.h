// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICharacterTypes.h"
#include "DebugUIManager.generated.h"

class UMyAIPluginSettings;
class UDebugAISettingsWidget;
class APlayerController;
class AMyAIController;

/**
 * Debug UI Manager: Toggles the AI Debug Widget and updates plugin settings at runtime.
 */
UCLASS()
class MYAIPLUGIN_API ADebugUIManager : public AActor
{
    GENERATED_BODY()

public:
    ADebugUIManager();

    /** Toggle visibility of debug widget */
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void ToggleDebugUI();

    UFUNCTION(BlueprintImplementableEvent, Category = "AI Debug")
    void OnDebugToggled(bool bNewState);


    /******************
    *   Combat & Patrol Styles   *
    ******************/
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetCombatStyle(EAICombatStyle NewStyle);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetPatrolStyle(EPatrolMode NewMode);

    /******************
    *    Patrol Settings    *
    ******************/
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetPatrolSpeed(float NewSpeed);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetPatrolInterval(float NewInterval);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetPatrolRadius(float NewRadius);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetPingPongSpline(bool bPingPong);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetSplineWaitTime(float NewWaitTime);

    /******************
    *    Combat Settings    *
    ******************/
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetEnableChasing(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetChaseDistance(float Distance);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetEnableAttacking(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetAttackRange(float Range);

    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetAttackCooldown(float Cooldown);

    /*****************
    *     Debug Settings     *
    *****************/
    UFUNCTION(BlueprintCallable, Category = "AI Debug")
    void SetEnableDebugLogs(bool bEnabled);

    /** Widget class to spawn at runtime */
    UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UDebugAISettingsWidget> DebugWidgetClass;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    AMyAIController* DebugAIControllerRef;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    /** Spawned instance of the debug UI widget */
    UPROPERTY(Transient)
    UDebugAISettingsWidget* DebugWidget;

    /** Runtime plugin settings access */
    UPROPERTY()
    UMyAIPluginSettings* RuntimeSettings;

    /** Tracks whether the debug UI is currently visible */
    bool bIsVisible = false;

    APlayerController* GetLocalPlayerController() const;
    void RefreshAIControllerPatrol();
};
