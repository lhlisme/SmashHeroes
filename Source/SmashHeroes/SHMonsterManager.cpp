// Fill out your copyright notice in the Description page of Project Settings.


#include "SHMonsterManager.h"


void USHMonsterManager::StartEnemySpawn(FName LevelName)
{
	if (!MonsterSpawnTable)
	{
		return;
	}

	// ��ʼ��SpawnBoxes
	SpawnBoxes.Empty();
	// ��ȡ�����е����й������ɺ�
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpawnBoxType, SpawnBoxes);


}

bool USHMonsterManager::GetRandomSpawnPoint(FTransform& SpawnPoint)
{
	return false;
}