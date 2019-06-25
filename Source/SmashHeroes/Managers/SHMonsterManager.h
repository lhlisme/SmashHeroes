// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MonsterCharacter.h"
#include "SHMonsterManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnGroupStruct
{
	GENERATED_USTRUCT_BODY()

public:
	FSpawnGroupStruct() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AMonsterCharacter>> Monsters;
};

USTRUCT(BlueprintType)
struct FWavesStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWavesStruct() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnGroupStruct> SpawnGroups;
};

/**
 * 怪物生成管理器
 */
UCLASS()
class SMASHHEROES_API USHMonsterManager : public UActorComponent
{
	GENERATED_BODY()

public:
	/** 场景怪物生成数据表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	UDataTable* MonsterSpawnTable;
	/** Monster生成盒类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	TSubclassOf<AActor> SpawnBoxType;

protected:
	/** 当前场景的所有怪物生成盒 */
	UPROPERTY(VisibleAnywhere, Category = "Monster Spawn")
	TArray<AActor*> SpawnBoxes;
	/** 生成的怪物数量 */
	UPROPERTY(VisibleAnywhere, Category = "Monster Spawn")
	int32 SpawnedMonsterCount = 0;
	/** 当前怪物波数 */
	UPROPERTY(VisibleAnywhere, Category = "Monster Spawn")
	int32 CurrentWave = 0;
	FWavesStruct MonsterWaves;

protected:
	/** 初始化状态 */
	void Init();

public:
	USHMonsterManager() {}
	/** 进入关卡时生成怪物 */
	UFUNCTION(BlueprintCallable)
	void StartEnemySpawn(FName LevelName);
	/** 获取随机生成位置 */
	UFUNCTION(BlueprintPure)
	bool GetRandomSpawnPoint(FTransform& SpawnPoint);
	/** 生成怪物 */
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	/** 获取已生成的怪物数量 */
	UFUNCTION(BlueprintCallable)
	int32 GetSpawnedMonsterCount();
};
