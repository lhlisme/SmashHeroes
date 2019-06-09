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
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(OwnerActor);
	if (OwnerCharacter) {
		if (OwnerCharacter->HasAuthority()) {
			// ��ʼ������
			SeekTarget = nullptr;
			AttackTarget = nullptr;
		
			// �����AI����ʼ��OwnerAIController
			if (IsAI) {
				OwnerAIController = Cast<AAIController>(OwnerCharacter->GetController());

				if (OwnerAIController) {
					// �����úڰ�ֵǰ������Ϊ��
					OwnerAIController->RunBehaviorTree(BehaviorTree);
					// ��ʼ���ڰ�ֵ��Ϣ
					InitBlackboard();
				}
			}
			
			// ��ʼ����ǰ��Ϊ��Ϣ
			SetBehavior(InitBehavior);
		}
	}
}

void UBehaviorComponent::InitBlackboard()
{
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
			if (OwnerActor) {
				// ����StartLocation
				Blackboard->SetValueAsVector(BBKey_StartLocation, OwnerActor->GetActorLocation());
				// ����TargetLocation
				Blackboard->SetValueAsVector(BBKey_TargetLocation, OwnerActor->GetActorLocation());
			}
			// ����IdleType
			Blackboard->SetValueAsEnum(BBKey_IdleType, (uint8)IdleType);
			// ����MaxRandLocationDistance
			Blackboard->SetValueAsFloat(BBKey_MaxRandLocationDistance, MaxRandLocationDistance);
			// ����RandLocationDelay
			Blackboard->SetValueAsFloat(BBKey_RandLocationDelay, RandLocationDelay);
			// ����FleeDistance
			Blackboard->SetValueAsFloat(BBKey_FleeDistance, FleeDistance);
			// ����MeleeAttackDistance
			Blackboard->SetValueAsFloat(BBKey_MeleeAttackDistance, MeleeAttackDistance);
			// ����RangeAttackDistance
			Blackboard->SetValueAsFloat(BBKey_RangeAttackDistance, RangeAttackDistance);
			// ����FollowDistance
			Blackboard->SetValueAsFloat(BBKey_FollowDistance, FollowDistance);
			// ����InvestigateDistance
			Blackboard->SetValueAsFloat(BBKey_InvestigateDistance, InvestigateDistance);
			// ����InvestigateInterval
			Blackboard->SetValueAsFloat(BBKey_InvestigateInterval, InvestigateInterval);
			// ����SeekAcceptanceRadius
			Blackboard->SetValueAsFloat(BBKey_SeekAcceptanceRadius, SeekAcceptanceRadius);
		}
	}
}

void UBehaviorComponent::SetSeekTarget(AActor* NewSeekTarget)
{
	SeekTarget = NewSeekTarget;
	// ���úڰ�TargetActor
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
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

	// ���û�����ã�Ѱ�����������SeekTargetTag��Actor��ΪĿ��
	SeekTarget = FindNearestTargetWithTag(SeekTargetTags, DistToTarget);
	// ���úڰ�TargetActor
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
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
	FVector ViewPoint = FVector(0.0f, 0.0f, 0.0f);	// ���߼�����ӵ㣬����(0,0,0)ʱʹ�õ�ǰ�鿴Ŀ����۾�λ��

	// ����AttackTargetTags��Ѱ����
	for (FName CurrentTag : AttackTargetTags) {
		TArray<AActor*> FindedActors;
		// ���ݵ�ǰTag��ȡ�����ڵ�������ض���
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors) {
			if (OwnerActor) {
				TempDistance = OwnerActor->GetDistanceTo(CurrentActor);
				// Ŀ������������
				if (TempDistance < InvestigateDistance) {
					// �����ҪĿ��ɼ����ܹ���
					if (IsRequireLineOfSight && OwnerAIController) {
						TargetInSight = OwnerAIController->LineOfSightTo(CurrentActor, ViewPoint, false);
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
	// ֻ�е�ǰ��Ϊ����ΪIdleʱ�Ÿ�����Ϊ
	if (CurrentBehavior != EBehaviorType::Idle) {
		return;
	}

	// ��Ŀ�����ľ���
	float DistToTarget = 0.0f;

	AttackTarget = FindAttackTarget(DistToTarget);
	// ����ҵ�����Ŀ�겢�ҵ�ǰ���Է�������
	if (AttackTarget) {
		// �ж��Ƿ�֧��Զ�̹���(��ս��������֧��)
		if (CanRangeAttack) {
			if (DistToTarget < MeleeAttackDistance) {
				// ��ʼ��ս����
				SetBehavior(EBehaviorType::MeleeAttack);
			}
			else if(DistToTarget < RangeAttackDistance){
				// ��ʼԶ�̹���
				SetBehavior(EBehaviorType::RangeAttack);
			}
			else {
				// ��ʼ׷��
				SetBehavior(EBehaviorType::Follow);
			}
		}
		else {
			if (DistToTarget < MeleeAttackDistance) {
				// ��ʼ��ս����
				SetBehavior(EBehaviorType::MeleeAttack);
			}
			else {
				// ��ʼ׷��
				SetBehavior(EBehaviorType::Follow);
			}
		}

		return;
	}

	SeekTarget = FindSeekTarget(DistToTarget);
	// ����ҵ�Ѱ��Ŀ��
	if (SeekTarget) {
		return;
	}

	// û��Ŀ�꣬����Ѳ��

}

