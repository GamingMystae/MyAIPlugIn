// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

/**
 * Actor that stores a sequence of patrol points and optional wait times
 * Used by AI Controllers to determine patrol routes
 */
UCLASS()
class MYAIPLUGIN_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

protected:
	virtual void BeginPlay() override;

public:
	/** Optional: Override if you actually use Tick() */
	virtual void Tick(float DeltaTime) override;

	/** List of patrol points (placed manually in the level) */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<AActor*> PatrolPoints;

	/** Optional wait time at each patrol point (same index as PatrolPoints) */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Patrol")
	TArray<float> PatrolWaitTimes;

	/** Helper function to validate patrol path setup */
	UFUNCTION(BlueprintCallable, Category = "Patrol")
	bool IsValidPath() const;
};

