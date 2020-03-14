// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.generated.h"


/** 怪物掉落物信息 */
USTRUCT(BlueprintType)
struct FLootStruct
{
	GENERATED_USTRUCT_BODY()

public:
	FLootStruct() {}

	/** 掉落物蓝图 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> LootBP;

	/** 最小掉落数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinDropCount = 0;

	/** 最大掉落数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDropCount = 1;

	/** 掉落率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropRate = 0.1f;
};

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

	/** 怪物掉落物信息 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLootStruct> Loots;
};

/** 怪物巡逻信息, 在SpawnBox中配置并初始化 */
USTRUCT(BlueprintType)
struct FMonsterPatrolInfoStruct
{
	GENERATED_USTRUCT_BODY()

public:
	FMonsterPatrolInfoStruct() {}

	/** 怪物巡逻类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPatrolType PatrolType;

	/** 怪物巡逻路线名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PatrolRouteName;
};

UCLASS()
class SMASHHEROES_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	/** 掉落物信息 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<FLootStruct> Loots;

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

	virtual bool TryEndGuard() override;

	/** 开始近战连击(怪物角色专用) */
	UFUNCTION(BlueprintCallable)
	bool StartMeleeCombo();

	/** 开始远程连击(怪物角色专用) */
	UFUNCTION(BlueprintCallable)
	bool StartRangeCombo();

	// 设置怪物基础信息
	void SetMonsterInfo(const FMonsterBaseInfoStruct& MonsterBaseInfo, const FMonsterPatrolInfoStruct& MonsterPatrolInfo);

	/** 生成掉落物 */
	UFUNCTION(BlueprintCallable)
	void SpawnLoot();
};

