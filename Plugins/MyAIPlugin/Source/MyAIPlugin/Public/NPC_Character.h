// Fill out your copyright notice in the Description page of Project Settings.

/********************************
** MY CUSTOM AI Character HEADER *
*********************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC_Character.generated.h"

class APatrolPath;
class ASplinePath;

/**
 * AI Character that can follow either a Patrol Path or Spline Path.
 * Controlled by MyAIController at runtime.
 */
UCLASS()
class MYAIPLUGIN_API ANPC_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC_Character();

	/** Assignable patrol path for waypoint-based AI movement */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	APatrolPath* PatrolPath;

	/** Assignable spline path for smooth AI movement */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	ASplinePath* SplinePath;

protected:
	virtual void BeginPlay() override;
};

