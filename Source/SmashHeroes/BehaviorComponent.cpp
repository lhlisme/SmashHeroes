// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
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

void UBehaviorComponent::SetSeekTarget(AActor* NewSeekTarget)
{
	SeekTarget = NewSeekTarget;
	// ���úڰ�TargetActor
	if (OwningAIController) {
		if (UBlackboardComponent* Blackboard = OwningAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, SeekTarget);
		}
	}
}

AActor* UBehaviorComponent::FindSeekTarget(float &DistToTarget)
{
	if (SeekTarget) {
		DistToTarget = OwningCharacter->GetDistanceTo(SeekTarget);
		return SeekTarget;
	}

	// ���û�����ã�Ѱ�����������SeekTargetTag��Actor��ΪĿ��
	SeekTarget = FindNearestTargetWithTag(SeekTargetTags, DistToTarget);
	// ���úڰ�TargetActor
	if (OwningAIController) {
		if (UBlackboardComponent* Blackboard = OwningAIController->GetBlackboardComponent()) {
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
		DistToTarget = OwningCharacter->GetDistanceTo(AttackTarget);
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

	// ��¼������Ŀ��ľ���
	DistToTarget = NearestDistance;
	// ���ù���Ŀ��
	AttackTarget = NearestTarget;
	// ���ºڰ�TargetActor��Ϣ
	if (OwningAIController) {
		if (UBlackboardComponent* Blackboard = OwningAIController->GetBlackboardComponent()) {
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
			if (OwningCharacter) {
				TempDistance = OwningCharacter->GetDistanceTo(CurrentActor);
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

void UBehaviorComponent::SetBehavior(EAIBehavior NewBehavior)
{
	if (OwningAIController) {
		if (UBlackboardComponent* Blackboard = OwningAIController->GetBlackboardComponent()) {
			Blackboard->SetValueAsEnum(BBKey_BehaviorType, (uint8)NewBehavior);
		}
	}
}

void UBehaviorComponent::UpdateBehavior()
{
	float DistToTarget = 0.0f;

	// ����ҵ�����Ŀ�겢�ҵ�ǰ���Է�������
	if (AttackTarget = FindAttackTarget(DistToTarget)) {
		// �ж��Ƿ�֧��Զ�̹���(��ս��������֧��)
		if (CanRangeAttack) {
			if (DistToTarget < MeleeAttackDistance) {
				// ��ʼ��ս����
				SetBehavior(EAIBehavior::MeleeAttack);
			}
			else if(DistToTarget < RangeAttackDistance){
				// ��ʼԶ�̹���
				SetBehavior(EAIBehavior::RangeAttack);
			}
			else {
				// ��ʼ׷��
				SetBehavior(EAIBehavior::Follow);
			}
		}
		else {
			if (DistToTarget < MeleeAttackDistance) {
				// ��ʼ��ս����
				SetBehavior(EAIBehavior::MeleeAttack);
			}
			else {
				// ��ʼ׷��
				SetBehavior(EAIBehavior::Follow);
			}
		}

		return;
	}


	// ����ҵ�Ѱ��Ŀ��
	if (SeekTarget = FindSeekTarget(DistToTarget)) {
		return;
	}
}

