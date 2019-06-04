// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.generated.h"

UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Patrol				UMETA(DisplayName = "Patrol"),
	MeleeAttack			UMETA(DisplayName = "MeleeAttack"),
	RangeAttack			UMETA(DisplayName = "RangeAttack"),
	Approach			UMETA(DisplayName = "Approach"),
	Flee				UMETA(DisplayName = "Flee"),
	StrafeAround		UMETA(DisplayName = "StrafeAround"),
	Hit					UMETA(DisplayName = "Hit")
};

UCLASS()
class SMASHHEROES_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	AMonsterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool Attack() override;
};
