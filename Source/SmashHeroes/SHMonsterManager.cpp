// Fill out your copyright notice in the Description page of Project Settings.


#include "SHMonsterManager.h"


void USHMonsterManager::StartEnemySpawn(FName LevelName)
{
	if (!MonsterSpawnTable)
	{
		return;
	}

	// 初始化SpawnBoxes
	SpawnBoxes.Empty();
	// 获取场景中的所有怪物生成盒
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnBoxType, SpawnBoxes);


}

bool USHMonsterManager::GetRandomSpawnPoint(FTransform& SpawnPoint)
{
	return false;
}