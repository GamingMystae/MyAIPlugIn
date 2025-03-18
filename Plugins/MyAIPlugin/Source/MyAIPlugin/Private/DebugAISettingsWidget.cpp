// Fill out your copyright notice in the Description page of Project Settings.

// Copyright Notice (optional)
// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugAISettingsWidget.h"
#include "DebugUIManager.h"
#include "MyAIPluginSettings.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"

void UDebugAISettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (DebugUIManagerRef)
    {
        UE_LOG(LogTemp, Log, TEXT("DebugAISettingsWidget: DebugUIManagerRef assigned successfully: %s"),
            *DebugUIManagerRef->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DebugAISettingsWidget: DebugUIManagerRef is NULL! Make sure it is set after widget creation."));
    }

    // Optional: sync UI with current settings
    UpdateUIFromSettings();

    // TODO: Bind your widget controls (checkboxes, sliders, etc.) to the below functions
}

void UDebugAISettingsWidget::ToggleDebugLogs(bool bEnable)
{
    if (DebugUIManagerRef)
    {
        DebugUIManagerRef->SetEnableDebugLogs(bEnable);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleDebugLogs: DebugUIManagerRef is null"));
    }
}

void UDebugAISettingsWidget::SetCombatStyle(EAICombatStyle NewStyle)
{
    if (DebugUIManagerRef)
    {
        DebugUIManagerRef->SetCombatStyle(NewStyle);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetCombatStyle: DebugUIManagerRef is null"));
    }
}

void UDebugAISettingsWidget::SetPatrolStyle(EPatrolMode NewMode)
{
    if (DebugUIManagerRef)
    {
        DebugUIManagerRef->SetPatrolStyle(NewMode);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetPatrolStyle: DebugUIManagerRef is null"));
    }
}

void UDebugAISettingsWidget::UpdateUIFromSettings()
{
    const UMyAIPluginSettings* Settings = UMyAIPluginSettings::GetMutable();
    if (!Settings) return;

    // Example: hook these to your actual UI widgets
    /*
    if (MyDebugLogCheckbox)
    {
        MyDebugLogCheckbox->SetIsChecked(Settings->bEnableDebugLogs);
    }

    if (MyPatrolSpeedSlider)
    {
        MyPatrolSpeedSlider->SetValue(Settings->PatrolMoveSpeed);
    }

    if (MyPatrolModeDropdown)
    {
        MyPatrolModeDropdown->SetSelectedIndex(static_cast<uint8>(Settings->PatrolMode));
    }
    */

    UE_LOG(LogTemp, Log, TEXT("DebugAISettingsWidget: UI synced with plugin settings."));
}
