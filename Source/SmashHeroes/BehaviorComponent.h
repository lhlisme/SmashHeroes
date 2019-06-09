// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorComponent.generated.h"


/** AI ��Ϊ���� */
UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Patrol				UMETA(DisplayName = "Patrol"),
	Seek				UMETA(DisplayName = "Seek"),
	MeleeAttack			UMETA(DisplayName = "MeleeAttack"),
	RangeAttack			UMETA(DisplayName = "RangeAttack"),
	Follow				UMETA(DisplayName = "Follow"),
	Flee				UMETA(DisplayName = "Flee"),
	Investigate			UMETA(DisplayName = "Investigate"),
	Evade				UMETA(DisplayName = "Evade"),
	Guard				UMETA(DisplayName = "Guard"),
	Hit					UMETA(DisplayName = "Hit"),
	Fall				UMETA(DisplayName = "Fall")
};

UENUM()
enum class EIdleType : uint8
{
	Stationary			UMETA(DisplayName = "Stationary"),
	RandomLocation		UMETA(DisplayName = "RandomLocation"),
	RandomWork			UMETA(DisplayName = "RandomWork")
};

UCLASS(ClassGroup=AIBehavior, hidecategories=(Object,LOD,Lighting,Transform,Sockets,TextureStreaming), editinlinenew, meta=(BlueprintSpawnableComponent))
class SMASHHEROES_API UBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	bool IsAI = false;	// �Ƿ���AI����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	UBehaviorTree* BehaviorTree;	// ��ǰAIʹ�õ���Ϊ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	EBehaviorType InitBehavior = EBehaviorType::Idle;	// ��ʼ��Ϊ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_BehaviorType = FName(TEXT("BehaviorType"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_TargetActor = FName(TEXT("TargetActor"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_StartLocation = FName(TEXT("StartLocation"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_TargetLocation = FName(TEXT("TargetLocation"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_IdleType = FName(TEXT("IdleType"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_MaxRandLocationDistance = FName(TEXT("MaxRandLocationDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_RandLocationDelay = FName(TEXT("RandLocationDelay"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_FleeDistance = FName(TEXT("FleeDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_MeleeAttackDistance = FName(TEXT("MeleeAttackDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_RangeAttackDistance = FName(TEXT("RangeAttackDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_FollowDistance = FName(TEXT("FollowDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_SeekAcceptanceRadius = FName(TEXT("SeekAcceptanceRadius"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_InvestigateDistance = FName(TEXT("InvestigateDistance"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_InvestigateInterval = FName(TEXT("InvestigateInterval"));	// �ڰ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle Settings")
	EIdleType IdleType = EIdleType::Stationary;	// ������Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle Settings")
	float MaxRandLocationDistance = 300.0f;	// �������߶�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle Settings")
	float RandLocationDelay = 10.0f;	// ����߶����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigate Settings")
	float InvestigateDistance = 3000.0f;	// ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigate Settings")
	float InvestigateInterval = 0.2f;	// �����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Settings")
	float FollowDistance = 4000.0f;	// ׷�پ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Settings")
	float FleeDistance = 1600.0f;	// �������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Settings")
	EBehaviorType FleeTransition = EBehaviorType::Idle;	// ������Ϊ������������һ��Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	TArray<FName> SeekTargetTags;	// Ѱ��Ŀ���Tag����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	float SeekAcceptanceRadius;		// Ѱ����Ϊ�Ŀɽ��ܰ뾶

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	EBehaviorType SeekTransition = EBehaviorType::Idle;		// Ѱ����Ϊ������������һ��Ϊ����

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	EBehaviorType MeleeAttackTransition = EBehaviorType::Idle;	// ��ս������Ϊ������������һ��Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	EBehaviorType RangeAttackTransition = EBehaviorType::Idle;	// Զ�̹�����Ϊ������������һ��Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guard Settings")
	EBehaviorType GuardTransition = EBehaviorType::Idle;	// ������Ϊ������������һ��Ϊ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Settings")
	EBehaviorType HitTransition = EBehaviorType::Idle;	// �ܻ���Ϊ������������һ��Ϊ����

private:
	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	AActor* OwnerActor;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	AAIController* OwnerAIController;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	EBehaviorType CurrentBehavior = EBehaviorType::Idle;	// ��ʼ��Ϊ

	UPROPERTY(VisibleAnywhere, Category = "Seek Settings")
	AActor* SeekTarget;		// Ѱ��Ŀ��(��ͬ��AttackTarget, ��Ҫͨ���ⲿָ��)

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	AActor* AttackTarget;

public:	
	// Sets default values for this component's properties
	UBehaviorComponent();

	/** ����Key��ʼ��Blackboard�е�ֵ */
	void InitBlackboard();

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

	/** ��ȡ��ǰ��Ϊ���� */
	UFUNCTION(BlueprintPure)
	EBehaviorType GetBehavior();

	/** ���úڰ���Ϊ��Ϣ */
	UFUNCTION(BlueprintCallable)
	void SetBehavior(EBehaviorType NewBehavior);

	/** ������Ϊ */
	UFUNCTION(BlueprintCallable)
	void UpdateBehavior();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
