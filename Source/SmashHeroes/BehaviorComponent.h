// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/BaseCharacter.h"
#include "AIController.h"
#include "BehaviorComponent.generated.h"


/** AI ��Ϊ���� */
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_BehaviorType = FName(TEXT("BehaviorType"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_TargetActor = FName(TEXT("TargetActor"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigate Settings")
	float InvestigateDistance = 3000.0f;	// ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Settings")
	float FollowDistance = 4000.0f;	// ׷�پ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Settings")
	float FleeDistance = 1600.0f;	// �������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	TArray<FName> SeekTargetTags;	// Ѱ��Ŀ���Tag����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	float SeekAcceptanceRadius;		// Ѱ����Ϊ�Ŀɽ��ܰ뾶

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float MeleeAttackDistance = 400.0f;	// ��ս��������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	bool CanRangeAttack = false;	// �Ƿ�֧��Զ�̹���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float RangeAttackDistance = 1600.0f;	// ��Χ��������

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	bool IsRequireLineOfSight = false;	// �����Ƿ���Ҫ��������Ұ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	TArray<FName> AttackTargetTags;	// �ɹ���Ŀ���Tag����

private:
	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	ABaseCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	AAIController* OwningAIController;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	EAIBehavior CurrentBehavior;	// ��ʼ��Ϊ

	UPROPERTY(VisibleAnywhere, Category = "Seek Settings")
	AActor* SeekTarget;		// Ѱ��Ŀ��(��ͬ��AttackTarget, ��Ҫͨ���ⲿָ��)

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	AActor* AttackTarget;

public:	
	// Sets default values for this component's properties
	UBehaviorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetOwningCharacter();

	/** ��ȡ��ǰ��Ϊ���� */
	UFUNCTION(BlueprintCallable)
	EAIBehavior GetCurrentBehavior();

	/** ����Ѱ��Ŀ��(�ⲿ����) */
	void SetSeekTarget(AActor* NewSeekTarget);

	/** ȷ��Ѱ��Ŀ�� */
	AActor* FindSeekTarget(float &DistToTarget);

	/** ��ȡѰ��Ŀ��(�ⲿ����) */
	AActor* GetSeekTarget();

	/** ȷ������Ŀ�� */
	AActor* FindAttackTarget(float &DistToTarget);

	/** ��ȡ��ǰ����Ŀ����� */
	UFUNCTION(BlueprintCallable)
	AActor* GetAttackTarget();

	/** ����ָ��Tag�ҵ������Ŀ�� */
	UFUNCTION(BlueprintCallable)
	AActor* FindNearestTargetWithTag(TArray<FName> TargerTags, float &DistToTarget);

	/** ���úڰ���Ϊ��Ϣ */
	UFUNCTION(BlueprintCallable)
	void SetBehavior(EAIBehavior NewBehavior);

	/** ������Ϊ */
	UFUNCTION(BlueprintCallable)
	void UpdateBehavior();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
