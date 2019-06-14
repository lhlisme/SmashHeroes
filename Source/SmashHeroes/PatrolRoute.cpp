// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolRoute.h"

// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PatrolBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("PatrolBillboard"));
	RootComponent = PatrolBillboard;
	PatrolSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));
	PatrolSpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
}

UBillboardComponent* APatrolRoute::GetBillboard()
{
	return PatrolBillboard;
}

USplineComponent* APatrolRoute::GetSpline()
{
	return PatrolSpline;
}

int32 APatrolRoute::GetSplinePointNum()
{
	if (PatrolSpline)
	{
		return PatrolSpline->GetNumberOfSplinePoints();
	}

	return 0;
}

FVector APatrolRoute::GetPatrolLocationByPointIndex(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace)
{
	if (PatrolSpline)
	{
		return PatrolSpline->GetLocationAtSplinePoint(PointIndex, CoordinateSpace);
	}

	return FVector(0.0f, 0.0f, 0.0f);
}

