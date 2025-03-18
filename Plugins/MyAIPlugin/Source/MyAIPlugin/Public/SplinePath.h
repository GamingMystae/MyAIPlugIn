// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplinePath.generated.h"

// Forward declaration
class USplineComponent;

/**
 * Actor that contains a spline used for AI pathing.
 * Includes per-point wait times used by AIController for timing behavior.
 */
UCLASS()
class MYAIPLUGIN_API ASplinePath : public AActor
{
	GENERATED_BODY()

public:
	ASplinePath();

	/** The spline component defining the path to follow */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* SplineComponent;

	/** Wait time (in seconds) for each spline point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Spline")
	TArray<float> WaitTimes;

	/** Get wait time for a spline index safely */
	UFUNCTION(BlueprintCallable, Category = "AI|Spline")
	float GetWaitTimeAtIndex(int32 Index) const;

protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
#endif
};
