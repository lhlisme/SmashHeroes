// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/PlayerController.h"
#include "SHPlayerControllerBase.generated.h"

/**
 * Base class for PlayerController, should be blueprinted
 */
UCLASS()
class SMASHHEROES_API ASHPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
private:
	float ForwardInput = 0.0f;
	float RightInput = 0.0f;

public:
	ASHPlayerControllerBase() {}

	virtual void BeginPlay() override;

};
