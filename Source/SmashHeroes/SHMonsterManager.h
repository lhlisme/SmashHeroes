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
	/** ���������������ݱ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	UDataTable* MonsterSpawnTable;
	/** Monster���ɺ����� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	TSubclassOf<AActor> SpawnBoxType;
	/** ��ǰ���������й������ɺ� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster Spawn")
	TArray<AActor*> SpawnBoxes;
public:
	/** ����ؿ�ʱ���ɵ��� */
	UFUNCTION(BlueprintCallable)
	void StartEnemySpawn(FName LevelName);
	/** ��ȡ�������λ�� */
	UFUNCTION(BlueprintPure)
	bool GetRandomSpawnPoint(FTransform& SpawnPoint);
};
