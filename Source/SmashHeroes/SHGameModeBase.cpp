// Fill out your copyright notice in the Description page of Project Settings.


#include "SHGameModeBase.h"
#include "SHGameStateBase.h"
#include "SHPlayerControllerBase.h"

ASHGameModeBase::ASHGameModeBase()
{
	GameStateClass = ASHGameStateBase::StaticClass();
	PlayerControllerClass = ASHPlayerControllerBase::StaticClass();

	// TODO ÍøÂç¸´ÖÆ?
	MonsterManager = CreateDefaultSubobject<USHMonsterManager>(TEXT("MonsterManager"));
}