// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/MonsterCharacter.h"
#include "SHMonsterManager.generated.h"

USTRUCT(BlueprintType)
struct FSpawnWaveStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AMonsterCharacter>> Monsters;
};

USTRUCT(BlueprintType)
struct FWavesStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnWaveStruct> SpawnWaves;
};

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHMonsterManager : public UObject
{
	GENERATED_BODY()

public:
	/** 场景怪物生成数据表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	UDataTable* MonsterSpawnTable;
	/** Monster生成盒类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	TSubclassOf<AActor> SpawnBoxType;
	/** 当前场景的所有怪物生成盒 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	TArray<AActor*> SpawnBoxes;
public:
	/** 进入关卡时生成敌人 */
	UFUNCTION(BlueprintCallable)
	void StartEnemySpawn(FName LevelName);
	/** 获取随机生成位置 */
	UFUNCTION(BlueprintPure)
	bool GetRandomSpawnPoint(FTransform& SpawnPoint);
};
