// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SHGameStateBase.generated.h"

/**
 * Base class for GameMode, should be blueprinted
 */
UCLASS()
class SMASHHEROES_API ASHGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	ASHGameStateBase() {}
};
