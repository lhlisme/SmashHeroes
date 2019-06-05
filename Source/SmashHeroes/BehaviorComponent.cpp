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
			// ��ʼ������
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
	FVector ViewPoint = FVector(0.0f, 0.0f, 0.0f);	// ���߼�����ӵ㣬����(0,0,0)ʱʹ�õ�ǰ�鿴Ŀ����۾�λ��

	// ����AttackTargetTags��Ѱ����
	for (FName CurrentTag : AttackTargetTags) {
		TArray<AActor*> FindedActors;
		// ���ݵ�ǰTag��ȡ�����ڵ�������ض���
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors) {
			if (OwningCharacter) {
				TempDistance = OwningCharacter->GetDistanceTo(CurrentActor);
				// Ŀ������������
				if (TempDistance < InvestigateDistance) {
					// �����ҪĿ��ɼ����ܹ���
					if (IsRequireLineOfSight && OwningAIController) {
						TargetInSight = OwningAIController->LineOfSightTo(CurrentActor, ViewPoint, false);
						// ���Ŀ�겻�����߷�Χ��, ������(����)
						if (!TargetInSight) {
							continue;
						}
					}
					EnemyCharacter = Cast<ABaseCharacter>(CurrentActor);
					// �����ǰĿ���ǽ�ɫ, �ж����Ƿ���(����Ŀ����ܲ���Character, ����ƻ���������)
					if (EnemyCharacter && !EnemyCharacter->IsAlive()) {
						continue;
					}
					// ��������ɹ���Ŀ����Ϣ
					if (TempDistance < NearestDistance) {
						NearestDistance = TempDistance;
						NearestTarget = CurrentActor;
					}
				}
			}
		}
	}

	// ���ù���Ŀ��
	AttackTarget = NearestTarget;
	// TODO ���ºڰ�TargetActor��Ϣ
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

