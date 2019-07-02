// Fill out your copyright notice in the Description page of Project Settings.


#include "SHMonsterManager.h"


void USHMonsterManager::Init()
{
	// 初始化已生成怪物数量
	SpawnedMonsterCount = 0;
	// 初始化SpawnBoxes
	SpawnBoxes.Empty();
	// 初始化当前怪物波数
	CurrentWave = 0;
}

void USHMonsterManager::StartEnemySpawn(FName LevelName)
{
	Init();

	if (!MonsterSpawnTable)
	{
		return;
	}

	// 获取当前关卡应该生成的所有怪物
	FString ContextString;
	FWavesStruct* MonsterWavesPtr = MonsterSpawnTable->FindRow<FWavesStruct>(LevelName, ContextString, true);
	// 如果没有怪物生成信息, 则直接返回
	if (!MonsterWavesPtr)
	{
		return;
	}
	MonsterWaves = *MonsterWavesPtr;

	// 获取场景中的所有怪物生成盒
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnBoxType, SpawnBoxes);
	// 生成怪物
	SpawnEnemy();
}

bool USHMonsterManager::GetRandomSpawnPoint(FTransform& SpawnPoint)
{
	
	return false;
}

void USHMonsterManager::SpawnEnemy()
{
	// 设置生成怪物时的碰撞检测参数
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// 当前波数没有超过配置的数量时, 生成一波
	if (CurrentWave < MonsterWaves.SpawnGroups.Num())
	{
		int32 RandomIndex = 0;
		for (auto& CurMonsterConfig : MonsterWaves.SpawnGroups[CurrentWave].Monsters)
		{
			// 所有生成盒都生成过对象后, 不继续生成
			if (SpawnBoxes.Num() <= 0)
			{
				break;
			}
			RandomIndex = FMath::RandRange(0, SpawnBoxes.Num() - 1);
			FTransform SpawnPoint = SpawnBoxes[RandomIndex]->GetActorTransform();
			AMonsterCharacter* SpawnedMonster = GetWorld()->SpawnActor<AMonsterCharacter>(CurMonsterConfig.MonsterBP, SpawnPoint, SpawnParams);
			if (SpawnedMonster)
			{
				++SpawnedMonsterCount;
				// 删除已经Spawn过怪物的生成盒
				SpawnBoxes.RemoveAtSwap(RandomIndex);
				// 初始化生成怪物信息
				SpawnedMonster->SetMonsterBaseInfo(CurMonsterConfig.MonsterInfo);
			}
		}
		++CurrentWave;
	}
}

int32 USHMonsterManager::GetSpawnedMonsterCount()
{
	return SpawnedMonsterCount;
}

int32 USHMonsterManager::GetKilledMonsterCount()
{
	return KilledMonsterCount;
}

void USHMonsterManager::IncreaseKilledMonsterCount()
{
	++KilledMonsterCount;
}