// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/BaseCharacter.h"
#include "AIController.h"
#include "BehaviorComponent.generated.h"


/** AI 行为类型 */
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
	EAIBehavior InitBehavior = EAIBehavior::Idle;	// 初始行为

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_BehaviorType = FName(TEXT("BehaviorType"));	// 黑板键名称

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	FName BBKey_TargetActor = FName(TEXT("TargetActor"));	// 黑板键名称

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Investigate Settings")
	float InvestigateDistance = 3000.0f;	// 侦查距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Settings")
	float FollowDistance = 4000.0f;	// 追踪距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee Settings")
	float FleeDistance = 1600.0f;	// 逃离距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	TArray<FName> SeekTargetTags;	// 寻找目标的Tag数组

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek Settings")
	float SeekAcceptanceRadius;		// 寻找行为的可接受半径

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float MeleeAttackDistance = 400.0f;	// 近战攻击距离

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	bool CanRangeAttack = false;	// 是否支持远程攻击

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	float RangeAttackDistance = 1600.0f;	// 范围攻击距离

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	bool IsRequireLineOfSight = false;	// 攻击是否需要考虑在视野内

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
	TArray<FName> AttackTargetTags;	// 可攻击目标的Tag数组

private:
	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	ABaseCharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	AAIController* OwningAIController;

	UPROPERTY(VisibleAnywhere, Category = "General Settings")
	EAIBehavior CurrentBehavior;	// 初始行为

	UPROPERTY(VisibleAnywhere, Category = "Seek Settings")
	AActor* SeekTarget;		// 寻找目标(不同于AttackTarget, 需要通过外部指定)

	UPROPERTY(VisibleAnywhere, Category = "Attack Settings")
	AActor* AttackTarget;

public:	
	// Sets default values for this component's properties
	UBehaviorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	ABaseCharacter* GetOwningCharacter();

	/** 获取当前行为类型 */
	UFUNCTION(BlueprintCallable)
	EAIBehavior GetCurrentBehavior();

	/** 设置寻找目标(外部调用) */
	void SetSeekTarget(AActor* NewSeekTarget);

	/** 确定寻找目标 */
	AActor* FindSeekTarget(float &DistToTarget);

	/** 获取寻找目标(外部调用) */
	AActor* GetSeekTarget();

	/** 确定攻击目标 */
	AActor* FindAttackTarget(float &DistToTarget);

	/** 获取当前攻击目标对象 */
	UFUNCTION(BlueprintCallable)
	AActor* GetAttackTarget();

	/** 根据指定Tag找到最近的目标 */
	UFUNCTION(BlueprintCallable)
	AActor* FindNearestTargetWithTag(TArray<FName> TargerTags, float &DistToTarget);

	/** 设置黑板行为信息 */
	UFUNCTION(BlueprintCallable)
	void SetBehavior(EAIBehavior NewBehavior);

	/** 更新行为 */
	UFUNCTION(BlueprintCallable)
	void UpdateBehavior();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
