// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/GameModeBase.h"
#include "Managers/SHMonsterManager.h"
#include "SHGameModeBase.generated.h"

/**
 * Base class for GameMode, should be blueprinted
 */
UCLASS()
class SMASHHEROES_API ASHGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	USHMonsterManager* MonsterManager;

public:
	ASHGameModeBase();
	
};
