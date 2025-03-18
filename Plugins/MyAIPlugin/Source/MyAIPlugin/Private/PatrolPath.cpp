// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
}

void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool APatrolPath::IsValidPath() const
{
	return PatrolPoints.Num() > 0 &&
		(PatrolWaitTimes.Num() == 0 || PatrolWaitTimes.Num() == PatrolPoints.Num());
}


