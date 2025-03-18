// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AICharacterTypes.h"
#include "DebugAISettingsWidget.generated.h"

class ADebugUIManager;
class AMyAIController;

UCLASS()
class MYAIPLUGIN_API UDebugAISettingsWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, meta = (ExposeOnSpawn), Category = "Debug")
    ADebugUIManager* DebugUIManagerRef;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "Debug")
    AMyAIController* DebugAIControllerRef;

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void ToggleDebugLogs(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void SetCombatStyle(EAICombatStyle NewStyle);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void SetPatrolStyle(EPatrolMode NewMode);

protected:
    virtual void NativeConstruct() override;

    /** Syncs UI widgets (checkboxes, sliders, etc.) with runtime settings */
    void UpdateUIFromSettings(); // <-- ADD THIS
};
