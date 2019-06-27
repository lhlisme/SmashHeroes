// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

bool AMonsterCharacter::MeleeAttack()
{
	AttackIndex = FMath::RandRange(1, MeleeAttackMontageMap.Num());

	return true;
}

bool AMonsterCharacter::RangeAttack()
{
	return true;
}

void AMonsterCharacter::SetMonsterBaseInfo(FMonsterBaseInfoStruct& MonsterInfo)
{
	if (BehaviorComponent)
	{
		BehaviorComponent->IdleType = MonsterInfo.IdleType;
		BehaviorComponent->PatrolType = MonsterInfo.PatrolType;
		BehaviorComponent->PatrolRouteName = MonsterInfo.PatrolRouteName;
	}
}