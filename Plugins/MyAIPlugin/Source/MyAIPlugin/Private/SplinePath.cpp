// Fill out your copyright notice in the Description page of Project Settings.


#include "SplinePath.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"

ASplinePath::ASplinePath()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and attach spline component
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
}

void ASplinePath::BeginPlay()
{
	Super::BeginPlay();
}

float ASplinePath::GetWaitTimeAtIndex(int32 Index) const
{
	return WaitTimes.IsValidIndex(Index) ? WaitTimes[Index] : 0.f;
}

#if WITH_EDITOR
void ASplinePath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const int32 NumPoints = SplineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < NumPoints; i++)
	{
		const FVector Pos = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		DrawDebugSphere(GetWorld(), Pos, 35.f, 12, FColor::Green, false, 5.f);
		DrawDebugString(GetWorld(), Pos + FVector(0, 0, 50.f), FString::Printf(TEXT("Wait: %.1fs"), GetWaitTimeAtIndex(i)));
	}
}
#endif
