﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 创建AI行为组件
	BehaviorComponent = CreateDefaultSubobject<UBehaviorComponent>(TEXT("BehaviorComponent"));
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMonsterCharacter::Attack()
{
	ComboIndex = FMath::RandRange(1, AttackMontageMap.Num());

	return true;
}


UBehaviorComponent* AMonsterCharacter::GetBehaviorComponent()
{
	return BehaviorComponent;
}

