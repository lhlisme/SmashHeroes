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

void UBehaviorComponent::Initialize()
{
	OwnerActor = GetOwner();
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(OwnerActor);
	if (OwnerCharacter && OwnerCharacter->HasAuthority()) {
		// 如果是AI，初始化OwnerAIController
		if (bIsAI)
		{
			OwnerAIController = Cast<AAIController>(OwnerCharacter->GetController());

			if (OwnerAIController)
			{
				// 在设置黑板值前运行行为树
				OwnerAIController->RunBehaviorTree(BehaviorTree);
				// 初始化黑板值信息
				InitBlackboard();
			}

			// 初始化巡逻路径
			TArray<AActor*> AllPatrolRoutes;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolRoute::StaticClass(), AllPatrolRoutes);
			for (AActor* CurrentPatrolRoute : AllPatrolRoutes)
			{
				if (CurrentPatrolRoute && PatrolRouteName.Compare(CurrentPatrolRoute->GetName()) == 0)
				{
					PatrolRoute = Cast<APatrolRoute>(CurrentPatrolRoute);
				}
			}
		}

		// 初始化当前行为信息
		ChangeBehavior(InitBehavior);
		// 初始化目标信息
		SeekTarget = nullptr;
		AttackTarget = nullptr;

		// 添加目标清除回调事件
		OnDeadBegin.AddDynamic(this, &UBehaviorComponent::ResetSeekTarget);
		OnDeadBegin.AddDynamic(this, &UBehaviorComponent::ResetAttackTarget);
		// 添加仇恨列表清空回调事件
		OnDeadBegin.AddDynamic(this, &UBehaviorComponent::ResetHateTargets);
	}
}

void UBehaviorComponent::InitBlackboard()
{
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
			if (OwnerActor) {
				// 设置StartLocation
				Blackboard->SetValueAsVector(BBKey_StartLocation, OwnerActor->GetActorLocation());
				// 设置TargetLocation
				Blackboard->SetValueAsVector(BBKey_TargetLocation, OwnerActor->GetActorLocation());
			}
			// 设置IdleType
			Blackboard->SetValueAsEnum(BBKey_IdleType, (uint8)IdleType);
			// 设置MaxRandLocationDistance
			Blackboard->SetValueAsFloat(BBKey_MaxRandLocationDistance, MaxRandLocationDistance);
			// 设置RandLocationDelay
			Blackboard->SetValueAsFloat(BBKey_RandLocationDelay, RandLocationDelay);
			// 设置FleeDistance
			Blackboard->SetValueAsFloat(BBKey_FleeDistance, FleeDistance);
			// 设置MeleeAttackDistance
			Blackboard->SetValueAsFloat(BBKey_MeleeAttackDistance, MeleeAttackDistance);
			// 设置RangeAttackDistance
			Blackboard->SetValueAsFloat(BBKey_RangeAttackDistance, RangeAttackDistance);
			// 设置FollowDistance
			Blackboard->SetValueAsFloat(BBKey_FollowDistance, FollowDistance);
			// 设置InvestigateDistance
			Blackboard->SetValueAsFloat(BBKey_InvestigateDistance, InvestigateDistance);
			// 设置InvestigateInterval
			Blackboard->SetValueAsFloat(BBKey_InvestigateInterval, InvestigateInterval);
			// 设置SeekAcceptanceRadius
			Blackboard->SetValueAsFloat(BBKey_SeekAcceptanceRadius, SeekAcceptanceRadius);
			// 设置FollowAcceptanceRadius
			Blackboard->SetValueAsFloat(BBKey_FollowAcceptanceRadius, FollowAcceptanceRadius);
		}
	}
}

void UBehaviorComponent::SetSeekTarget(AActor* NewSeekTarget)
{
	SeekTarget = NewSeekTarget;
	// 设置黑板TargetActor
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, SeekTarget);
		}
	}
}

void UBehaviorComponent::ResetSeekTarget()
{
	if (bIsAI)
	{
		SeekTarget = nullptr;
		// 设置黑板TargetActor
		if (OwnerAIController) {
			UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
			if (Blackboard) {
				Blackboard->SetValueAsObject(BBKey_TargetActor, SeekTarget);
			}
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

void UBehaviorComponent::AddHateTarget(AActor* NewTarget)
{
	float* TargetHateValue = HateTargets.Find(NewTarget);
	// 如果NewTarget不存在, 则添加并初始化仇恨值
	if (!TargetHateValue)
	{
		HateTargets.Add(NewTarget, InitHateValue);
	}
}

void UBehaviorComponent::UpdateHateTargets()
{
	float TempDistance = 0.0f;
	bool bTargetInSight = false;
	FVector ViewPoint = FVector(0.0f, 0.0f, 0.0f);	// 视线检测用视点，传入(0,0,0)时使用当前查看目标的眼睛位置
	ABaseCharacter* EnemyCharacter = nullptr;

	// 根据AttackTargetTags搜寻敌人, 更新仇恨列表
	for (FName CurrentTag : AttackTargetTags)
	{
		TArray<AActor*> FindedActors;
		// 根据当前Tag获取场景内的所有相关对象
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors)
		{
			if (OwnerActor)
			{
				TempDistance = OwnerActor->GetDistanceTo(CurrentActor);
				// 目标在追踪距离内
				if (TempDistance < FollowDistance)
				{
					// 如果当前目标是角色, 判断其是否存活(攻击目标可能不是Character, 如可破坏场景物体)
					EnemyCharacter = Cast<ABaseCharacter>(CurrentActor);
					if (EnemyCharacter && !EnemyCharacter->IsAlive())
					{
						// 如果目标不存活, 但位于仇恨列表里, 则移除
						RemoveHateTarget(CurrentActor);
						continue;
					}

					// 如果需要目标可见才能攻击
					if (IsRequireLineOfSight)
					{
						if (OwnerAIController)
						{
							bTargetInSight = OwnerAIController->LineOfSightTo(CurrentActor, ViewPoint, false);
							// 如果目标不在视线范围内, 则跳过(忽略)
							if (!bTargetInSight)
							{
								continue;
							}
							// 添加仇恨目标并初始化仇恨值, 如果其已存在, 不做处理
							AddHateTarget(CurrentActor);
						}
					}
					else
					{
						// 添加仇恨目标并初始化仇恨值, 如果其已存在, 不做处理
						AddHateTarget(CurrentActor);
					}
				}
				else
				{
					// 如果目标不在追踪距离内, 判断其是否位于仇恨列表, 是的话需要删除
					RemoveHateTarget(CurrentActor);
				}
			}
		}
	}
}

void UBehaviorComponent::UpdateTargetHateValue(AActor *NewTarget, float HateValue)
{
	float* TargetHateValue = HateTargets.Find(NewTarget);
	float OldHateValue = *TargetHateValue;
	// 如果NewTarget已存在, 则更新仇恨值
	if (TargetHateValue)
	{
		(*TargetHateValue) += HateValue;
		UE_LOG(LogTemp, Log, TEXT("OldHateValue: %f, NewHateValue: %f"), OldHateValue, *(TargetHateValue));
	}
	else
	{
		// 如果NewTarget不存在, 则添加
		HateTargets.Add(NewTarget, HateValue);
	}
}

void UBehaviorComponent::RemoveHateTarget(AActor *Target)
{
	bool bTargetExist = HateTargets.Find(Target) != nullptr ? true : false;
	if (bTargetExist)
	{
		HateTargets.Remove(Target);
	}
}

AActor* UBehaviorComponent::GetHatestNearTarget(float &DistToTarget)
{
	// 寻找仇恨值最大的目标
	AActor* HatestNearTarget = nullptr;
	float TempDistance = 0.0f;
	float MaxHateValue = FLT_MIN;
	float CurrentDistance = FLT_MAX;	// 当前仇恨值最大目标的距离
	
	for (auto& CurItem : HateTargets)
	{
		// 仇恨值相同的情况下优先寻找距离更近的目标
		if (FMath::IsNearlyEqual(CurItem.Value, MaxHateValue, 0.1f))
		{
			if (OwnerActor)
			{
				TempDistance = OwnerActor->GetDistanceTo(CurItem.Key);
				if (TempDistance < CurrentDistance)
				{
					HatestNearTarget = CurItem.Key;
					CurrentDistance = TempDistance;
				}
			}
		}
		else if (CurItem.Value > MaxHateValue)
		{
			HatestNearTarget = CurItem.Key;
			MaxHateValue = CurItem.Value;
			if (OwnerActor)
			{
				CurrentDistance = OwnerActor->GetDistanceTo(CurItem.Key);
			}
		}
	}

	DistToTarget = CurrentDistance;
	return HatestNearTarget;
}

void UBehaviorComponent::ResetHateTargets()
{
	HateTargets.Empty();
}

AActor* UBehaviorComponent::FindAttackTarget(float &DistToTarget)
{
	ABaseCharacter* EnemyCharacter = nullptr;

	// 找到仇恨值最高的最近目标, 并记录到攻击目标的距离
	AttackTarget = GetHatestNearTarget(DistToTarget);
	// 更新黑板TargetActor信息
	if (OwnerAIController) {
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard) {
			Blackboard->SetValueAsObject(BBKey_TargetActor, AttackTarget);
		}
	}

	return AttackTarget;
}

void UBehaviorComponent::ResetAttackTarget()
{
	// 如果当前对象由AI控制，在攻击结束时，重置攻击目标
	if (bIsAI) {
		AttackTarget = nullptr;
		// 更新黑板TargetActor信息
		if (OwnerAIController) {
			UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
			if (Blackboard) {
				Blackboard->SetValueAsObject(BBKey_TargetActor, AttackTarget);
			}
		}
	}
}

AActor* UBehaviorComponent::GetAttackTarget()
{
	return AttackTarget;
}

AActor* UBehaviorComponent::FindNearestTargetWithTag(TArray<FName> TargerTags, float &DistToTarget)
{
	if (TargerTags.Num() < 0)
	{
		return nullptr;
	}

	AActor* NearestActor = nullptr;
	float NearestDistance = FLT_MAX;
	float TempDistance = 0.0f;

	for (FName CurrentTag : TargerTags)
	{
		TArray<AActor*> FindedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), CurrentTag, FindedActors);

		for (AActor* CurrentActor : FindedActors)
		{
			if (OwnerActor)
			{
				TempDistance = OwnerActor->GetDistanceTo(CurrentActor);
				if (TempDistance < NearestDistance)
				{
					NearestDistance = TempDistance;
					NearestActor = CurrentActor;
				}
			}
		}
	}

	DistToTarget = NearestDistance;
	return NearestActor;
}

void UBehaviorComponent::FindNextPatrolLocation()
{
	// 设置目标巡逻位置
	int32 PointIndex = 0;
	switch (PatrolType)
	{
	case EPatrolType::Single:
	{
		++PatrolSplineIndex;
		if (PatrolSplineIndex >= PatrolRoute->GetSplinePointNum())
		{
			// 已到达目的地, 结束巡逻
			bIsPatrolEnded = true;
			TransitionBehavior();
			return;
		}
		PointIndex = FMath::Clamp(PatrolSplineIndex, 0, PatrolRoute->GetSplinePointNum());
		UE_LOG(LogTemp, Log, TEXT("Single Patrol Point Index: %d"), PointIndex);
		break;
	}
	case EPatrolType::Looping:
	{
		++PatrolSplineIndex;
		PointIndex = PatrolSplineIndex % PatrolRoute->GetSplinePointNum();
		UE_LOG(LogTemp, Log, TEXT("Looping Patrol Point Index: %d"), PointIndex);
		break;
	}
	case EPatrolType::BackAndForth:
	{
		if (PatrolDirection)
		{
			// 正向巡逻
			++PatrolSplineIndex;
			// 如果到达了最终点, 则调转方向
			if (PatrolSplineIndex >= PatrolRoute->GetSplinePointNum() - 1)
			{
				PatrolDirection = false;
			}
		}
		else
		{
			// 反向巡逻
			--PatrolSplineIndex;
			// 如果到达了最初点, 则调转方向
			if (PatrolSplineIndex <= 0)
			{
				PatrolDirection = true;
			}
		}
		PointIndex = PatrolSplineIndex;
		UE_LOG(LogTemp, Log, TEXT("BackAndForth Patrol Point Index: %d"), PointIndex);
		break;
	}
	}

	FVector TargetPatrolLocation = PatrolRoute->GetPatrolLocationByPointIndex(PointIndex, ESplineCoordinateSpace::World);
	if (OwnerAIController)
	{
		if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent())
		{
			Blackboard->SetValueAsVector(BBKey_TargetLocation, TargetPatrolLocation);
		}
	}
}

EBehaviorType UBehaviorComponent::GetBehavior()
{
	return CurrentBehavior;
}

bool UBehaviorComponent::ChangeBehavior(EBehaviorType NewBehavior)
{
	if (OwnerActor && OwnerActor->HasAuthority()) {
		// 如果新的Behavior与当前Behavior相同,无需修改
		if (NewBehavior == CurrentBehavior) {
			return false;
		}

		// 调用当前行为的结束事件
		switch (CurrentBehavior)
		{
		case EBehaviorType::Idle:
			OnIdleEnd.Broadcast();
			break;
		case EBehaviorType::Patrol:
			OnPatrolEnd.Broadcast();
			break;
		case EBehaviorType::Seek:
			OnSeekEnd.Broadcast();
			break;
		case EBehaviorType::MeleeAttack:
			OnMeleeAttackEnd.Broadcast();
			break;
		case EBehaviorType::RangeAttack:
			OnRangeAttackEnd.Broadcast();
			break;
		case EBehaviorType::Follow:
			OnFollowEnd.Broadcast();
			break;
		case EBehaviorType::Flee:
			OnFleeEnd.Broadcast();
			break;
		case EBehaviorType::Investigate:
			OnInvestigateEnd.Broadcast();
			break;
		case EBehaviorType::Evade:
			OnEvadeEnd.Broadcast();
			break;
		case EBehaviorType::Guard:
			OnGuardEnd.Broadcast();
			break;
		case EBehaviorType::Hit:
			OnHitEnd.Broadcast();
			break;
		case EBehaviorType::Fall:
			OnFallEnd.Broadcast();
			break;
		case EBehaviorType::Dead:
			OnDeadEnd.Broadcast();
			break;
		default:
			break;
		}

		// 更新当前行为信息
		CurrentBehavior = NewBehavior;
		if (OwnerAIController)
		{
			if (UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent())
			{
				Blackboard->SetValueAsEnum(BBKey_CurrentBehaviorType, (uint8)NewBehavior);
			}
		}

		// 调用新行为的开始事件
		switch (NewBehavior)
		{
		case EBehaviorType::Idle:
			OnIdleBegin.Broadcast();
			break;
		case EBehaviorType::Patrol:
			OnPatrolBegin.Broadcast();
			break;
		case EBehaviorType::Seek:
			OnSeekBegin.Broadcast();
			break;
		case EBehaviorType::MeleeAttack:
			OnMeleeAttackBegin.Broadcast();
			break;
		case EBehaviorType::RangeAttack:
			OnRangeAttackBegin.Broadcast();
			break;
		case EBehaviorType::Follow:
			OnFollowBegin.Broadcast();
			break;
		case EBehaviorType::Flee:
			OnFleeBegin.Broadcast();
			break;
		case EBehaviorType::Investigate:
			OnInvestigateBegin.Broadcast();
			break;
		case EBehaviorType::Evade:
			OnEvadeBegin.Broadcast();
			break;
		case EBehaviorType::Guard:
			OnGuardBegin.Broadcast();
			break;
		case EBehaviorType::Hit:
			OnHitBegin.Broadcast();
			break;
		case EBehaviorType::Fall:
			OnFallBegin.Broadcast();
			break;
		case EBehaviorType::Dead:
			OnDeadBegin.Broadcast();
			break;
		default:
			break;
		}

		return true;
	}

	return false;
}

void UBehaviorComponent::UpdateBehavior()
{
	ABaseCharacter* OwnerCharacter = nullptr;
	if (OwnerActor)
	{
		OwnerCharacter = Cast<ABaseCharacter>(OwnerActor);
		if (!OwnerCharacter)
		{
			return;
		}
	}

	// 死亡状态判断
	if (!OwnerCharacter->IsAlive()) {
		SetTargetBehavior(EBehaviorType::Dead);
		return;
	}

	// 如果刚由其他行为过渡至另一行为, 则跳过本次更新, 以直接进入其子树
	if (bIsTransition)
	{
		bIsTransition = false;
		return;
	}

	// 如果血量过低, 触发逃离
	if (bCanFlee && OwnerCharacter->GetHealthPercentage() <= FleeHealthThreshold)
	{
		SetTargetBehavior(EBehaviorType::Flee);
		return;
	}

	// 更新仇恨列表(仇恨列表对象均在FollowDistance内)
	UpdateHateTargets();

	// 到目标对象的距离
	float DistToTarget = 0.0f;
	AttackTarget = FindAttackTarget(DistToTarget);
	// 如果找到攻击目标
	if (AttackTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("AttackTarget: %s"), *(AttackTarget->GetName()));
		// 判断是否支持远程攻击(近战攻击必须支持)
		if (CanRangeAttack)
		{
			if (DistToTarget < MeleeAttackDistance)
			{
				// 准备开始近战攻击
				SetTargetBehavior(EBehaviorType::MeleeAttack);
			}
			else if (DistToTarget < RangeAttackDistance)
			{
				// 准备开始远程攻击
				SetTargetBehavior(EBehaviorType::RangeAttack);
			}
			else
			{
				// 准备开始追踪
				SetTargetBehavior(EBehaviorType::Follow);
			}
		}
		else
		{
			if (DistToTarget < MeleeAttackDistance)
			{
				UE_LOG(LogTemp, Log, TEXT("MeleeAttack"));
				// 准备开始近战攻击
				SetTargetBehavior(EBehaviorType::MeleeAttack);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Follow"));
				// 准备开始追踪
				SetTargetBehavior(EBehaviorType::Follow);
			}
		}

		return;
	}

	SeekTarget = FindSeekTarget(DistToTarget);
	// 如果找到寻找目标
	if (SeekTarget)
	{
		return;
	}

	// 没有目标, 判断是否可以进行巡逻
	if (PatrolType != EPatrolType::Disabled)
	{
		if (!bIsPatrolEnded)
		{
			// 准备开始巡逻
			SetTargetBehavior(EBehaviorType::Patrol);
			return;
		}
	}
	
	// 不能巡逻, 进入Idle状态
	SetTargetBehavior(EBehaviorType::Idle);
}

void UBehaviorComponent::TransitionBehavior()
{
	EBehaviorType TransitionTo = EBehaviorType::Idle;

	switch (CurrentBehavior)
	{
	case EBehaviorType::Patrol:
		TransitionTo = PatrolTransition;
		break;
	case EBehaviorType::Flee:
		TransitionTo = FleeTransition;
		break;
	case EBehaviorType::MeleeAttack:
		TransitionTo = MeleeAttackTransition;
		break;
	case EBehaviorType::RangeAttack:
		TransitionTo = RangeAttackTransition;
		break;
	case EBehaviorType::Hit:
		TransitionTo = HitTransition;
		break;
	case EBehaviorType::Guard:
		TransitionTo = GuardTransition;
		break;
	case EBehaviorType::Evade:
		TransitionTo = EvadeTransition;
		break;
	default:
		break;
	}

	SetTargetBehavior(TransitionTo);
	ChangeBehavior(TransitionTo);
	// 设置过渡标记
	bIsTransition = true;
}

void UBehaviorComponent::SetTargetBehavior(EBehaviorType NewBehavior)
{
	// 更新目标行为
	TargetBehavior = NewBehavior;
	// 更新黑板信息
	if (OwnerAIController)
	{
		UBlackboardComponent* Blackboard = OwnerAIController->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsEnum(BBKey_TargetBehaviorType, (uint8)NewBehavior);
		}
	}
}

void UBehaviorComponent::BeginMeleeAttack()
{
	ChangeBehavior(EBehaviorType::MeleeAttack);
}

void UBehaviorComponent::EndMeleeAttack()
{
	ChangeBehavior(MeleeAttackTransition);
}

void UBehaviorComponent::BeginRangeAttack()
{
	ChangeBehavior(EBehaviorType::RangeAttack);
}

void UBehaviorComponent::EndRangeAttack()
{
	ChangeBehavior(RangeAttackTransition);
}

void UBehaviorComponent::BeginEvade()
{
	ChangeBehavior(EBehaviorType::Evade);
}

void UBehaviorComponent::EndEvade()
{
	ChangeBehavior(EvadeTransition);
}

void UBehaviorComponent::BeginGuard()
{
	ChangeBehavior(EBehaviorType::Guard);
}

void UBehaviorComponent::EndGuard()
{
	ChangeBehavior(GuardTransition);
}

void UBehaviorComponent::BeginHit()
{
	ChangeBehavior(EBehaviorType::Hit);
}

void UBehaviorComponent::EndHit()
{
	ChangeBehavior(HitTransition);
}

void UBehaviorComponent::BeginDead()
{
	ChangeBehavior(EBehaviorType::Dead);
}

void UBehaviorComponent::EndDead()
{
	ChangeBehavior(DeadTransition);
}

void UBehaviorComponent::UpdateMoveSpeed()
{
	if (OwnerActor)
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(OwnerActor);
		if (OwnerCharacter)
		{
			switch (CurrentBehavior)
			{
			case EBehaviorType::Follow:
				OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FRandRange(MinFollowMoveSpeed, MaxFollowMoveSpeed);
				break;
			case EBehaviorType::Patrol:
				OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FRandRange(MinPatrolMoveSpeed, MaxPatrolMoveSpeed);
				break;
			case EBehaviorType::Flee:
				OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FRandRange(MinFleeMoveSpeed, MaxFleeMoveSpeed);
				break;
			case EBehaviorType::Seek:
				OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FRandRange(MinSeekMoveSpeed, MaxSeekMoveSpeed);
				break;
			default:
				break;
			}
		}
	}
}

