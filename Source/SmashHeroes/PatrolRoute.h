// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BillboardComponent.h"
#include "Components/SplineComponent.h"
#include "PatrolRoute.generated.h"

UCLASS()
class SMASHHEROES_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* PatrolBillboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USplineComponent* PatrolSpline;

public:	
	// Sets default values for this actor's properties
	APatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	UBillboardComponent* GetBillboard();
	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSpline();
	UFUNCTION(BlueprintCallable)
	int32 GetSplinePointNum();
	UFUNCTION(BlueprintCallable)
	FVector GetPatrolLocationByPointIndex(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace);
};
