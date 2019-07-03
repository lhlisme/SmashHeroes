// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.generated.h"


/** 需要通过表格配置的怪物基本信息(即同一蓝图间可能有区别的属性) */
USTRUCT(BlueprintType)
struct FMonsterBaseInfoStruct
{
	GENERATED_USTRUCT_BODY()

public:
	FMonsterBaseInfoStruct() {}

	/** 怪物Idle类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EIdleType IdleType;

	/** 怪物巡逻类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPatrolType PatrolType;

	/** 怪物巡逻路径名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PatrolRouteName;

};

UCLASS()
class SMASHHEROES_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AMonsterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool MeleeAttack() override;

	virtual bool RangeAttack() override;

	// 设置怪物基础信息
	void SetMonsterBaseInfo(FMonsterBaseInfoStruct& MonsterInfo);

	/** 生成掉落物 */
	/*UFUNCTION(BlueprintCallable)
	void SpawnLoot();*/
};

