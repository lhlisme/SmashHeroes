// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Characters/MonsterCharacter.h"
#include "SpawnBox.generated.h"

UCLASS()
class SMASHHEROES_API ASpawnBox : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* Billboard;

	/** 生成盒对应的巡逻信息 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolInfo")
	FMonsterPatrolInfoStruct PatrolInfo;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ASpawnBox();

};
