// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorComponent.generated.h"


UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Patrol				UMETA(DisplayName = "Patrol"),
	Seek				UMETA(DisplayName = "Seek"),
	MeleeAttack			UMETA(DisplayName = "MeleeAttack"),
	RangeAttack			UMETA(DisplayName = "RangeAttack"),
	Follow				UMETA(DisplayName = "Follow"),
	Flee				UMETA(DisplayName = "Flee"),
	Investigate			UMETA(DisplayName = "Investigate"),
	Defend				UMETA(DisplayName = "Defend"),
	Hit					UMETA(DisplayName = "Hit")
};

UENUM()
enum class EIdleType : uint8
{
	Stationary			UMETA(DisplayName = "Stationary"),
	RandomLocation		UMETA(DisplayName = "RandomLocation"),
	RandomWork			UMETA(DisplayName = "RandomWork")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMASHHEROES_API UBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	EAIBehavior InitBehavior = EAIBehavior::Idle;	// ��ʼ��Ϊ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigate Settings")
	float InvestigateDistance = 3000.0f;	// ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Settings")
	float FollowDistance = 4000.0f;	// ׷�پ���



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float MeleeAttackDistance = 400.0f;	// ��ս��������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float RangeAttackDistance = 1600.0f;	// ��Χ��������

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	bool IsRequireLineOfSight = false;	// �����Ƿ���Ҫ��������Ұ��

private:
	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	AActor *AttackTarget;

public:	
	// Sets default values for this component's properties
	UBehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
