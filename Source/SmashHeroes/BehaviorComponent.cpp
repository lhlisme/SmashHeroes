// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values for this component's properties
UBehaviorComponent::UBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(OwnerActor);
	if (OwnerCharacter) {
		OwnerAIController = Cast<AAIController>(OwnerCharacter->GetController());
		
		if (OwnerCharacter->HasAuthority()) {
			// 初始化数据
			CurrentBehavior = InitBehavior;
			SeekTarget = nullptr;
			AttackTarget = nullptr;
		}
	}
}


// Called every frame
void UBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBehaviorComponent::SetSeekTarget(AActor* NewSeekTarget)
{
	SeekTarget = NewSeekTarget;
	// 设置黑板TargetActor
	if (OwnerAIController) {
		if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, SeekTarget);
		}
	}
}

AActor* UBehaviorComponent::FindSeekTarget(float &DistToTarget)
{
	if (SeekTarget) {
		DistToTarget = OwnerActor->GetDistanceTo(SeekTarget);
		return SeekTarget;
	}

	// 如果没有设置，寻找最近的满足SeekTargetTag的Actor作为目标
	SeekTarget = FindNearestTargetWithTag(SeekTargetTags, DistToTarget);
	// 设置黑板TargetActor
	if (OwnerAIController) {
		if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, SeekTarget);
		}
	}

	return SeekTarget;
}

AActor* UBehaviorComponent::GetSeekTarget()
{
	return SeekTarget;
}

AActor* UBehaviorComponent::FindAttackTarget(float &DistToTarget)
{
	if (AttackTarget) {
		DistToTarget = OwnerActor->GetDistanceTo(AttackTarget);
		return AttackTarget;
	}

	AActor* NearestTarget = nullptr;
	ABaseCharacter* EnemyCharacter = nullptr;
	float NearestDistance = 99999997952.0f;
	float TempDistance = 0.0f;
	bool TargetInSight = false;
	FVector ViewPoint = FVector(0.0f, 0.0f, 0.0f);	// 视线检测用视点，传入(0,0,0)时使用当前查看目标的眼睛位置

	// 根据AttackTargetTags搜寻敌人
	for (FName CurrentTag : AttackTargetTags) {
		TArray<AActor*> FindedActors;
		// 根据当前Tag获取场景内的所有相关对象
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors) {
			if (OwnerActor) {
				TempDistance = OwnerActor->GetDistanceTo(CurrentActor);
				// 目标在侦测距离内
				if (TempDistance < InvestigateDistance) {
					// 如果需要目标可见才能攻击
					if (IsRequireLineOfSight && OwnerAIController) {
						TargetInSight = OwnerAIController->LineOfSightTo(CurrentActor, ViewPoint, false);
						// 如果目标不在视线范围内, 则跳过(忽略)
						if (!TargetInSight) {
							continue;
						}
					}
					EnemyCharacter = Cast<ABaseCharacter>(CurrentActor);
					// 如果当前目标是角色, 判断其是否存活(攻击目标可能不是Character, 如可破坏场景物体)
					if (EnemyCharacter && !EnemyCharacter->IsAlive()) {
						continue;
					}
					// 更新最近可攻击目标信息
					if (TempDistance < NearestDistance) {
						NearestDistance = TempDistance;
						NearestTarget = CurrentActor;
					}
				}
			}
		}
	}

	// 记录到攻击目标的距离
	DistToTarget = NearestDistance;
	// 设置攻击目标
	AttackTarget = NearestTarget;
	// 更新黑板TargetActor信息
	if (OwnerAIController) {
		if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, AttackTarget);
		}
	}

	return AttackTarget;
}

AActor* UBehaviorComponent::GetAttackTarget()
{
	return AttackTarget;
}

AActor* UBehaviorComponent::FindNearestTargetWithTag(TArray<FName> TargerTags, float &DistToTarget)
{
	if (TargerTags.Num() < 0) {
		return nullptr;
	}

	AActor* NearestActor = nullptr;
	float NearestDistance = 99999997952.0f;
	float TempDistance = 0.0f;

	for (FName CurrentTag : TargerTags) {
		TArray<AActor*> FindedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors) {
			if (OwnerActor) {
				TempDistance = OwnerActor->GetDistanceTo(CurrentActor);
				if (TempDistance < NearestDistance) {
					NearestDistance = TempDistance;
					NearestActor = CurrentActor;
				}
			}
		}
	}

	DistToTarget = NearestDistance;
	return NearestActor;
}

EBehaviorType UBehaviorComponent::GetBehavior()
{
	return CurrentBehavior;
}

void UBehaviorComponent::SetBehavior(EBehaviorType NewBehavior)
{
	CurrentBehavior = NewBehavior;
	if (OwnerAIController) {
		if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsEnum(BBKey_BehaviorType, (uint8)NewBehavior);
		}
	}
}

void UBehaviorComponent::UpdateBehavior()
{
	// 只有当前行为类型为Idle时才更新行为
	if (CurrentBehavior != EBehaviorType::Idle) {
		return;
	}

	// 到目标对象的距离
	float DistToTarget = 0.0f;

	AttackTarget = FindAttackTarget(DistToTarget);
	// 如果找到攻击目标并且当前可以发动攻击
	if (AttackTarget) {
		// 判断是否支持远程攻击(近战攻击必须支持)
		if (CanRangeAttack) {
			if (DistToTarget < MeleeAttackDistance) {
				// 开始近战攻击
				SetBehavior(EBehaviorType::MeleeAttack);
			}
			else if(DistToTarget < RangeAttackDistance){
				// 开始远程攻击
				SetBehavior(EBehaviorType::RangeAttack);
			}
			else {
				// 开始追踪
				SetBehavior(EBehaviorType::Follow);
			}
		}
		else {
			if (DistToTarget < MeleeAttackDistance) {
				// 开始近战攻击
				SetBehavior(EBehaviorType::MeleeAttack);
			}
			else {
				// 开始追踪
				SetBehavior(EBehaviorType::Follow);
			}
		}

		return;
	}

	SeekTarget = FindSeekTarget(DistToTarget);
	// 如果找到寻找目标
	if (SeekTarget) {
		return;
	}

	// 没有目标，进行巡逻

}

