// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorComponent.h"
#include "Kismet/GameplayStatics.h"

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

	OwningCharacter = Cast<ABaseCharacter>(GetOwner());
	if (OwningCharacter) {
		OwningAIController = Cast<AAIController>(OwningCharacter->GetController());
		
		if (OwningCharacter->HasAuthority()) {
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

ABaseCharacter* UBehaviorComponent::GetOwningCharacter()
{
	return OwningCharacter;
}

EAIBehavior UBehaviorComponent::GetCurrentBehavior()
{
	return CurrentBehavior;
}

AActor* UBehaviorComponent::GetAttackTarget()
{
	return AttackTarget;
}

AActor* UBehaviorComponent::FindAttackTarget()
{
	if (AttackTarget) {
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
			if (OwningCharacter) {
				TempDistance = OwningCharacter->GetDistanceTo(CurrentActor);
				// 目标在侦测距离内
				if (TempDistance < InvestigateDistance) {
					// 如果需要目标可见才能攻击
					if (IsRequireLineOfSight && OwningAIController) {
						TargetInSight = OwningAIController->LineOfSightTo(CurrentActor, ViewPoint, false);
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

	// 设置攻击目标
	AttackTarget = NearestTarget;
	// TODO 更新黑板TargetActor信息
	return AttackTarget;
}

AActor* UBehaviorComponent::FindNearestTargetWithTag(TArray<FName> TargerTags)
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
			if (OwningCharacter) {
				TempDistance = OwningCharacter->GetDistanceTo(CurrentActor);
				if (TempDistance < NearestDistance) {
					NearestDistance = TempDistance;
					NearestActor = CurrentActor;
				}
			}
		}
	}

	return NearestActor;
}

