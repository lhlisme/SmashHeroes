// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Abilities/SHGameplayAbility.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component, and set it to be explicitly replicated
	AbilitySystem = CreateDefaultSubobject<USHAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);
	// Create the attribute set, this replicates by default
	CharacterAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));

	CharacterLevel = 1;
	bAbilitiesInitialized = false;

	// 创建行为组件
	BehaviorComponent = CreateDefaultSubobject<UBehaviorComponent>(TEXT("BehaviorComponent"));
	BehaviorComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 生成武器
	GenerateWeapon();
	// 初始化行为组件
	BehaviorComponent->Initialize();
	// 添加攻击状态初始化回调事件
	BehaviorComponent->OnMeleeAttackEnd.AddDynamic(this, &ABaseCharacter::ResetAttackStatus);
	BehaviorComponent->OnEvadeBegin.AddDynamic(this, &ABaseCharacter::ResetAttackStatus);
	BehaviorComponent->OnGuardBegin.AddDynamic(this, &ABaseCharacter::ResetAttackStatus);
	// 初始化命中检测信息
	InitHitCheckInfo();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 死亡状态判断
	if (BehaviorComponent->GetBehavior() != EBehaviorType::Dead && !IsAlive())
	{
		BehaviorComponent->BeginDead();
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	// 初始化所有Abilities
	if (AbilitySystem) {
		AbilitySystem->InitAbilityActorInfo(this, this);
		AddStartupGameplayAbilities();
	}
}

void ABaseCharacter::UnPossessed()
{
	// TODO 清理背包
}

void ABaseCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes.
	// As pawns may be spawned before the client controller possesses them especially in a multiplayer enviroment.
	if (AbilitySystem) {
		AbilitySystem->RefreshAbilityActorInfo();
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CharacterLevel);
}

void ABaseCharacter::GenerateWeapon()
{
	switch (ArmedState)
	{
	case EArmedState::DualWield: {
		LeftWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		RightWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (LeftWeapon && RightWeapon) {
			LeftWeapon->SetActorRelativeLocation(LeftRelativeLocation);
			LeftWeapon->SetActorRelativeRotation(LeftRelativeRotation);
			RightWeapon->SetActorRelativeLocation(RightRelativeLocation);
			RightWeapon->SetActorRelativeRotation(RightRelativeRotation);
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LeftWeaponSocket);
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, RightWeaponSocket);
			LeftWeapon->InitialWeapon();
			RightWeapon->InitialWeapon();
		}
		break;
	}
	case EArmedState::LeftHold: {
		LeftWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (LeftWeapon) {
			LeftWeapon->SetActorRelativeLocation(LeftRelativeLocation);
			LeftWeapon->SetActorRelativeRotation(LeftRelativeRotation);
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, LeftWeaponSocket);
			LeftWeapon->InitialWeapon();
		}
		break;
	}
	case EArmedState::RightHold: {
		RightWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (RightWeapon) {
			RightWeapon->SetActorRelativeLocation(RightRelativeLocation);
			RightWeapon->SetActorRelativeRotation(RightRelativeRotation);
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, RightWeaponSocket);
			RightWeapon->InitialWeapon();
		}
		break;
	}
	default:
		break;
	}
}

void ABaseCharacter::DestroyWeapon()
{
	if (LeftWeapon)
	{
		LeftWeapon->Destroy();
	}

	if (RightWeapon)
	{
		RightWeapon->Destroy();
	}
}

bool ABaseCharacter::TryAttack()
{
	if (AbilitySystem && CanUseAnyAbility())
	{
		if (AttackType == EAttackType::MeleeAttack)
		{
			return AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::MeleeAttack));
		}
		else
		{
			return AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::RangeAttack));
		}
	}

	return false;
}

bool ABaseCharacter::TryMeleeAttack()
{
	if (AbilitySystem && CanUseAnyAbility())
	{
		bool bIsSuccess = AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::MeleeAttack));

		if (bIsSuccess)
		{
			// 初始化命中标记
			bIsAttackHit = false;
		}

		return bIsSuccess;
	}

	return false;
}

bool ABaseCharacter::TryRangeAttack()
{
	if (AbilitySystem && CanUseAnyAbility())
	{
		bool bIsSuccess = AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::RangeAttack));
	
		if (bIsSuccess)
		{
			// 初始化命中标记
			bIsAttackHit = false;
		}

		return bIsSuccess;
	}

	return false;
}

bool ABaseCharacter::MeleeAttack()
{
	// 返回值表示是否有效执行
	return false;
}

bool ABaseCharacter::RangeAttack()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetAttackMontage()
{
	if (AttackInfo)
	{
		return AttackInfo->AttackMontage;
	}

	return nullptr;
}

void ABaseCharacter::HandleHit(float DamageAmount, AActor* DamageCauser, FLinearColor InLinearColor)
{
	if (HitReactions.Num() > 0)
	{
		// 当前对应的受击反馈类型
		EHitReaction FirstHitReaction = PopHitReaction();

		// 当前对应的受击动画
		UAnimMontage** HitMontagePtr = HitMontageMap.Find(FirstHitReaction);

		if (HitMontagePtr)
		{
			// 若攻击被格挡, 仍处于防御状态, 否则进入受击状态
			if (FirstHitReaction != EHitReaction::BlockHit)
			{
				// 进入受击状态
				BehaviorComponent->BeginHit();
			}

			PlayAnimMontage(*HitMontagePtr);
		}

		// 更新仇恨值
		UpdateHateValue(DamageAmount, DamageCauser);

		// 播放受击特效
		PlayEmissiveEffect(InLinearColor);
	}
}

void ABaseCharacter::UpdateHateValue(float DamageAmount, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("On Damaged RestHealth: %f, DamageAmount: %f"), CharacterAttributeSet->GetHealth(), DamageAmount);
	// 如果当前对象是AI, 则添加仇恨目标(调用前应判断是否存活)
	if (BehaviorComponent && BehaviorComponent->bIsAI)
	{
		// 目前直接使用人物受到的伤害作为仇恨值增量
		BehaviorComponent->UpdateTargetHateValue(DamageCauser, DamageAmount);
	}
}

// TODO 增加组队概念时, 需要考虑玩家之间的队伍关系
bool ABaseCharacter::IsTargetHostile(AActor* TargetActor)
{
	if (BehaviorComponent)
	{
		for (auto CurTag : BehaviorComponent->AttackTargetTags)
		{
			if (TargetActor->ActorHasTag(CurTag))
			{
				return true;
			}
		}
	}

	return false;
}

TArray<FTransform> ABaseCharacter::GetProjectileSpawnTransforms(FGameplayTag ProjectileTag)
{
	TArray<FTransform> SpawnTransformList;

	FSHProjectileSpawnInfos* MatchedInfos = ProjectileSpawnMap.Find(ProjectileTag);
	if (MatchedInfos)
	{
		for (FSHProjectileSpawnInfo& CurSpawnInfo : MatchedInfos->SpawnInfoList)
		{
			if (CurSpawnInfo.SpawnType == EProjectileSpawnType::SpawnOnLeft)
			{
				if (!LeftWeapon)
				{
					continue;
				}
				// 从左手武器上的Socket生成投射物
				FTransform SpawnTransform = LeftWeapon->GetSocketTransformByName(CurSpawnInfo.SpawnSocketName);

				if (CurSpawnInfo.RotationType == EProjectileRotationType::UseInstigatorRotation)
				{
					SpawnTransform.SetRotation(GetActorRotation().Quaternion());
				}

				SpawnTransformList.Add(SpawnTransform);
			}
			else if (CurSpawnInfo.SpawnType == EProjectileSpawnType::SpawnOnRight)
			{
				if (!RightWeapon)
				{
					continue;
				}
				// 从右手武器上的Socket生成投射物
				FTransform SpawnTransform = RightWeapon->GetSocketTransformByName(CurSpawnInfo.SpawnSocketName);

				if (CurSpawnInfo.RotationType == EProjectileRotationType::UseInstigatorRotation)
				{
					SpawnTransform.SetRotation(GetActorRotation().Quaternion());
				}

				SpawnTransformList.Add(SpawnTransform);
			}
			else
			{
				USkeletalMeshComponent* CharacterMesh = GetMesh();
				if (CharacterMesh)
				{
					// 从Character自身骨架上的Socket生成投射物
					FTransform SpawnTransform = CharacterMesh->GetSocketTransform(CurSpawnInfo.SpawnSocketName);

					if (CurSpawnInfo.RotationType == EProjectileRotationType::UseInstigatorRotation)
					{
						SpawnTransform.SetRotation(GetActorRotation().Quaternion());
					}

					SpawnTransformList.Add(SpawnTransform);
				}
			}
		}
	}

	return SpawnTransformList;
}

void ABaseCharacter::PushHitReaction(EHitReaction NewHitReaction)
{
	HitReactions.Push(NewHitReaction);
}

EHitReaction ABaseCharacter::PopHitReaction()
{
	// 弹出队首的HitReaction并返回
	EHitReaction FirstHitReaction = EHitReaction::HitLeft;

	if (HitReactions.Num() > 0)
	{
		FirstHitReaction = HitReactions[0];
		HitReactions.RemoveAt(0);
	}

	return FirstHitReaction;
}

void ABaseCharacter::PushDefaultHitReaction(EHitReaction NewDefaultHitReaction)
{
	DefaultHitReactions.Push(NewDefaultHitReaction);
}

EHitReaction ABaseCharacter::PopDefaultHitReaction()
{
	// 弹出队首的DefaultHitReaction并返回
	EHitReaction FirstDefaultHitReaction = EHitReaction::HitLeft;

	if (DefaultHitReactions.Num() > 0)
	{
		FirstDefaultHitReaction = DefaultHitReactions[0];
		DefaultHitReactions.RemoveAt(0);
	}

	return FirstDefaultHitReaction;
}

void ABaseCharacter::PrintHitReaction(EHitReaction InHitReaction)
{
	switch (InHitReaction)
	{
	case EHitReaction::BlockHit:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  BlockHit"));
		break;
	case EHitReaction::HitLeft:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  HitLeft"));
		break;
	case EHitReaction::HitRight:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  HitRight"));
		break;
	case EHitReaction::GuardBreak:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  GuardBreak"));
		break;
	case EHitReaction::KnockBack:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  KnockBack"));
		break;
	case EHitReaction::KnockDown:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  KnockDown"));
		break;
	case EHitReaction::KnockUp:
		UE_LOG(LogTemp, Log, TEXT("HitReaction:  KnockUp"));
		break;
	}
}

bool ABaseCharacter::IsForcedHitReaction(EHitReaction InHitReaction)
{
	if (InHitReaction == EHitReaction::KnockBack || InHitReaction == EHitReaction::KnockDown || InHitReaction == EHitReaction::KnockUp)
	{
		return true;
	}

	return false;
}

ERelativeOrientation ABaseCharacter::CalculateRelativeOrientation(FVector TargetLocation)
{
	// 默认在前方
	ERelativeOrientation RelativeOrientation = ERelativeOrientation::Forward;

	FRotator DeltaRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation) - GetActorRotation();

	UE_LOG(LogTemp, Log, TEXT("DeltaRotator: %f"), DeltaRotator.Yaw);
	if (DeltaRotator.Yaw <= 45.0f)
	{
		if (DeltaRotator.Yaw > -45.0f)
		{
			RelativeOrientation = ERelativeOrientation::Forward;
		}
		else if (DeltaRotator.Yaw > -135.0f)
		{
			RelativeOrientation = ERelativeOrientation::Left;
		}
		else
		{
			RelativeOrientation = ERelativeOrientation::Backward;
		}
	}
	else
	{
		if (DeltaRotator.Yaw <= 135.0f)
		{
			RelativeOrientation = ERelativeOrientation::Right;
		}
		else
		{
			RelativeOrientation = ERelativeOrientation::Backward;
		}
	}

	return RelativeOrientation;
}

void ABaseCharacter::CalculateHitDirection(const FRotator& DeltaRotator, EHitReaction& HitReaction)
{
	if (DeltaRotator.Yaw > 0.0f)
	{
		HitReaction = EHitReaction::HitRight;
		UE_LOG(LogTemp, Log, TEXT("HitRight  DeltaRotator: %f"), DeltaRotator.Yaw);
	}
	else
	{
		HitReaction = EHitReaction::HitLeft;
		UE_LOG(LogTemp, Log, TEXT("HitLeft  DeltaRotator: %f"), DeltaRotator.Yaw);
	}
}

bool ABaseCharacter::IsHitInDefenseRange(const FRotator& DeltaRotator, EHitReaction& HitReaction)
{
	bool bHitInDefenseRange = false;

	// 如果受击点处于防御范围内
	if (DeltaRotator.Yaw > -0.5f * GetDefenseRange() && DeltaRotator.Yaw <= 0.5f * GetDefenseRange())
	{
		bHitInDefenseRange = true;
		HitReaction = EHitReaction::BlockHit;
	}

	return bHitInDefenseRange;
}

void ABaseCharacter::CheckHitResult(AActor* DamageCauser, FVector HitLocation, float DefenseFactor, float& DamageDone, float& EnergyCost, float& StiffCost)
{
	// 获取默认受击反馈
	EHitReaction HitReaction = PopDefaultHitReaction();
	bool bIsBlocked = false;	// 本次攻击是否被成功格挡
	float MaxHealth = GetMaxHealth();
	float MaxEnergy = GetMaxEnergy();

	// 被击者朝向受击点的旋转
	FRotator TargetToHit = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitLocation);
	// 受击点相对当前人物(未转向攻击者)的旋转
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(TargetToHit, GetActorRotation());

	// 如果当前处于防御状态，并且受击点在防御范围内(防御时不消耗硬直系数)
	if (BehaviorComponent->GetBehavior() == EBehaviorType::Guard && IsHitInDefenseRange(DeltaRotator, HitReaction))
	{
		float ScaleFactor = 0.5f;
		// 剩余能量值
		float RestEnergy = GetEnergy();

		// 根据伤害计算消耗的Energy
		EnergyCost = DamageDone * ScaleFactor * MaxEnergy / MaxHealth;

		// 用于格挡伤害的剩余能量值不足
		if (EnergyCost > RestEnergy)
		{
			// 剩余能量值可影响的伤害
			float DamageBlocked = RestEnergy * MaxHealth / (ScaleFactor *  MaxEnergy);
			DamageDone = (DamageDone - DamageBlocked) + DamageBlocked * (1.0f - DefenseFactor);
			// 防御击破
			HitReaction = EHitReaction::GuardBreak;
		}
		else
		{
			// 防御成功格挡大部分伤害
			DamageDone *= (1.0f - DefenseFactor);
		}
	}
	else // 未成功格挡攻击
	{
		// 剩余硬直系数
		float RestStiff = GetStiffFactor();
		// 硬直系数是否消耗完毕
		bool IsStiffExhausted = false;

		if (BehaviorComponent->bIsAI)
		{
			// 计算消耗的硬直系数(仅针对AI对象)
			StiffCost = DamageDone * (CharacterLevel + 2) * 0.5f / MaxHealth * 100.0f;
			IsStiffExhausted = StiffCost > RestStiff ? true : false;
			UE_LOG(LogTemp, Log, TEXT("Hit %s -- StiffCost: %f, RestStiff: %f"), *GetFName().ToString(), StiffCost, RestStiff);
			// 硬直消耗完毕, 通过定时器重新恢复硬直值
			if (IsStiffExhausted && !bStiffRecovering)
			{
				GetWorldTimerManager().SetTimer(StiffRecoverTimerHandle, this, &ABaseCharacter::RecoverStiffFactor, GetStiffRecoverTime(), false);
				bStiffRecovering = true;
				UE_LOG(LogTemp, Log, TEXT("%s Start Recover Stiff Factor"), *GetFName().ToString());
			}
		}

		// 如果是玩家对象或者硬直系数消耗完毕
		if (!BehaviorComponent->bIsAI || IsStiffExhausted)
		{
			if (DamageCauser)
			{
				// 受击后先转向攻击者(仅在Yaw轴旋转, 以防止目标在空中导致异常翻转的情况)
				SetActorRotation(FRotator(0.f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageCauser->GetActorLocation()).Yaw, 0.f).Quaternion());
			}

			// 若HitReaction是强制性受击反馈类型, 则无需修改
			if (!IsForcedHitReaction(HitReaction))
			{
				// 计算转向后新的相对旋转
				DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(TargetToHit, GetActorRotation());
				CalculateHitDirection(DeltaRotator, HitReaction);
			}
		}
		else
		{
			// 不播放受击动作
			HitReaction = EHitReaction::NoReaction;
		}
	}

	// 添加受击反馈
	PushHitReaction(HitReaction);
}

void ABaseCharacter::RecoverStiffFactor()
{
	CharacterAttributeSet->SetStiffFactor(CharacterAttributeSet->GetMaxStiffFactor());

	// 恢复完毕后清除定时器
	GetWorldTimerManager().ClearTimer(StiffRecoverTimerHandle);
	bStiffRecovering = false;
	UE_LOG(LogTemp, Log, TEXT("%s Stiff Factor Recovered.  %f"), *GetFName().ToString(), GetStiffFactor());
}

bool ABaseCharacter::TryEvade()
{
	if (AbilitySystem && CanUseAnyAbility())
	{
		return AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::Evade));
	}

	return false;
}

bool ABaseCharacter::Evade()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetEvadeMontage()
{
	// TODO 根据和目标的相对位置朝不同地方闪避
	return EvadeMontage;
}

bool ABaseCharacter::TryStartGuard()
{
	if (AbilitySystem && CanUseAnyAbility())
	{
		return AbilitySystem->TryActivateAbilityByClass(GetAbilityClassByType(EAbilityType::Guard));
	}

	return false;
}

bool ABaseCharacter::TryEndGuard()
{
	if (BehaviorComponent && GetCurrentBehavior() == EBehaviorType::Guard)
	{
		BehaviorComponent->EndGuard();

		return true;
	}

	return false;
}

bool ABaseCharacter::Guard()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetGuardMontage()
{
	return GuardMontage;
}

void ABaseCharacter::InitHitCheckInfo()
{
	// 根据Socket名称初始化对应位置信息
	if (LeftWeapon)
	{
		LeftWeapon->HitCheckInfo.InitSocketLocations();
	}

	if (RightWeapon)
	{
		RightWeapon->HitCheckInfo.InitSocketLocations();
	}

	for (auto& InfoPair : HitCheckInfoMap)
	{
		InfoPair.Value.InitSocketLocations();
	}
}

void ABaseCharacter::UpdateHitCheckInfo(const bool CheckLeft, const bool CheckRight, const bool CheckBody, const TArray<FName>& BodySocketNames)
{
	if (CheckLeft && LeftWeapon)
	{
		LeftWeapon->HitCheckInfo.ClearDamagedActors();
		LeftWeapon->HitCheckInfo.UpdateSocketLocations(LeftWeapon->GetWeaponMesh());
	}

	if (CheckRight && RightWeapon)
	{
		RightWeapon->HitCheckInfo.ClearDamagedActors();
		RightWeapon->HitCheckInfo.UpdateSocketLocations(RightWeapon->GetWeaponMesh());
	}

	if (CheckBody)
	{
		FSHHitCheckInfo* TargetInfo = nullptr;
		for (int32 i = 0; i < BodySocketNames.Num(); ++i)
		{
			TargetInfo = HitCheckInfoMap.Find(BodySocketNames[i]);
			if (TargetInfo)
			{
				TargetInfo->ClearDamagedActors();
				TargetInfo->UpdateSocketLocations(GetMesh());
			}
		}
	}
}

bool ABaseCharacter::MeleeAttackCheck(const EAttackStrength AttackStrength, const bool CheckLeft, const bool CheckRight, const bool CheckBody, const TArray<FName>& BodySocketNames, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, EHitReaction DefaultHitReaction, FVector InImpulse, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData)
{
	bool bIsHit = false;	// 是否命中目标

	// 近战攻击检测
	if (LeftWeapon && CheckLeft)
	{
		LeftWeapon->HitCheckInfo.HitCheck(this, AttackStrength, ObjectTypes, ActorsToIgnore, DrawDebugType, TraceColor, TraceHitColor, DrawTime, LeftWeapon->GetWeaponMesh(), LeftWeapon->SurfaceHitEffects, DefaultHitReaction, InImpulse, FinalOutHits, HitTargetsData, bIsHit);
	}

	if (RightWeapon && CheckRight)
	{
		RightWeapon->HitCheckInfo.HitCheck(this, AttackStrength, ObjectTypes, ActorsToIgnore, DrawDebugType, TraceColor, TraceHitColor, DrawTime, RightWeapon->GetWeaponMesh(), RightWeapon->SurfaceHitEffects, DefaultHitReaction, InImpulse, FinalOutHits, HitTargetsData, bIsHit);
	}

	if (CheckBody)
	{
		FSHHitCheckInfo* CurCheckInfo = nullptr;
		for (int32 i = 0; i < BodySocketNames.Num(); ++i)
		{
			CurCheckInfo = HitCheckInfoMap.Find(BodySocketNames[i]);
			if (!CurCheckInfo)
			{
				continue;
			}

			CurCheckInfo->HitCheck(this, AttackStrength, ObjectTypes, ActorsToIgnore, DrawDebugType, TraceColor, TraceHitColor, DrawTime, GetMesh(), SurfaceHitEffects, DefaultHitReaction, InImpulse, FinalOutHits, HitTargetsData, bIsHit);
		}
	}

	// 设置当前攻击是否命中标记
	bIsAttackHit |= bIsHit;

	return bIsHit;
}

void ABaseCharacter::ResetAttackStatus()
{
	// 如果攻击结束或被中断的话, 重新初始化AttackIndex
	AttackIndex = 0;
	AttackInfo = nullptr;
}

float ABaseCharacter::GetHealth() const
{
	return CharacterAttributeSet->GetHealth();
}

float ABaseCharacter::GetMaxHealth() const
{
	return CharacterAttributeSet->GetMaxHealth();
}

float ABaseCharacter::GetHealthPercentage() const
{
	return CharacterAttributeSet->GetHealth() / CharacterAttributeSet->GetMaxHealth();
}

float ABaseCharacter::GetEnergy() const
{
	return CharacterAttributeSet->GetEnergy();
}

float ABaseCharacter::GetMaxEnergy() const
{
	return CharacterAttributeSet->GetMaxEnergy();
}

float ABaseCharacter::GetEnergyPercentage() const
{
	return CharacterAttributeSet->GetEnergy() / CharacterAttributeSet->GetMaxEnergy();
}

float ABaseCharacter::GetMoveSpeed() const
{
	return CharacterAttributeSet->GetMoveSpeed();
}

float ABaseCharacter::GetDefenseRange() const
{
	return CharacterAttributeSet->GetDefenseRange();
}

float ABaseCharacter::GetStiffFactor() const
{
	return CharacterAttributeSet->GetStiffFactor();
}

float ABaseCharacter::GetMaxStiffFactor() const
{
	return CharacterAttributeSet->GetMaxStiffFactor();
}

float ABaseCharacter::GetStiffFactorPercentage() const
{
	return CharacterAttributeSet->GetStiffFactor() / CharacterAttributeSet->GetMaxStiffFactor();
}

float ABaseCharacter::GetStiffRecoverTime() const
{
	return CharacterAttributeSet->GetStiffRecoverTime();
}

int32 ABaseCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool ABaseCharacter::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0) {
		// 等级改变后, 需要刷新玩家的能力
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return GetHealth() > 0.0f;
}

void ABaseCharacter::PlayEmissiveEffect(FLinearColor EmissiveEffectColor)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	if (CharacterMesh)
	{
		CharacterMesh->SetVectorParameterValueOnMaterials(FName(TEXT("EmissiveEffectColor")), FVector(EmissiveEffectColor));
		CharacterMesh->SetScalarParameterValueOnMaterials(FName(TEXT("EmissiveEffectStartTime")), UGameplayStatics::GetTimeSeconds(GetWorld()));
	}
}

void ABaseCharacter::PlayDissolveEffect(float DissolveLength, FLinearColor DissolveColor)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	if (CharacterMesh)
	{
		CharacterMesh->SetVectorParameterValueOnMaterials(FName(TEXT("DissolveLength")), FVector(DissolveLength));
		CharacterMesh->SetVectorParameterValueOnMaterials(FName(TEXT("DissolveColor")), FVector(DissolveColor));
		CharacterMesh->SetScalarParameterValueOnMaterials(FName(TEXT("DissolveStartTime")), UGameplayStatics::GetTimeSeconds(GetWorld()));
	}
}

bool ABaseCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystem)
	{
		return AbilitySystem->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void ABaseCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<USHGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystem) {
		AbilitySystem->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool ABaseCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystem && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray<TPair<float, float>> DurationAndTimeRemaining = AbilitySystem->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}

bool ABaseCharacter::CanUseAnyAbility()
{
	return IsAlive() && !UGameplayStatics::IsGamePaused(GetWorld());
}

bool ABaseCharacter::IsUsingSpecificAbility(FGameplayTagContainer AbilityTags)
{
	if (AbilitySystem) {
		TArray<USHGameplayAbility*> ActiveAbilities;
		AbilitySystem->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);

		return ActiveAbilities.Num() > 0;
	}

	return false;
}

TSubclassOf<class USHGameplayAbility> ABaseCharacter::GetAbilityClassByType(EAbilityType InAbilityType)
{
	TSubclassOf<class USHGameplayAbility>* AbilityClass = CharacterAbilities.Find(InAbilityType);
	if (AbilityClass)
	{
		return *AbilityClass;
	}

	return nullptr;
}

void ABaseCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystem);

	if (Role == ROLE_Authority && !bAbilitiesInitialized) {
		// 仅在服务器端发授能力
		for (auto& StartupAbilityPair : CharacterAbilities) {
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(StartupAbilityPair.Value, GetCharacterLevel(), INDEX_NONE, this));
		}

		// 授予被动能力
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects) {
			FGameplayEffectContextHandle EffectContext = AbilitySystem->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystem->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid()) {
				FActiveGameplayEffectHandle ActivateGEHandle = AbilitySystem->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystem);
			}
		}

		//AddSlottedGameplayAbilities()
		bAbilitiesInitialized = true;
	}
}

void ABaseCharacter::RemoveStartupGameplayAbilities()
{
	check(AbilitySystem);

	if (Role == ROLE_Authority && bAbilitiesInitialized) {
		// 移除上一次调用中添加的所有能力
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystem->GetActivatableAbilities()) {
			if ((Spec.SourceObject == this) && CheckSpecificAbility(Spec.Ability->GetClass())) {
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i) {
			AbilitySystem->ClearAbility(AbilitiesToRemove[i]);
		}

		// 移除所有应用在Character身上的被动效果
		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystem->RemoveActiveEffects(Query);

		// RemoveSlottedGameplayAbilities(true);

		bAbilitiesInitialized = false;
	}
}

bool ABaseCharacter::CheckSpecificAbility(TSubclassOf<class USHGameplayAbility> InAbilityClass)
{
	for (auto& AbilityPair : CharacterAbilities)
	{
		if (AbilityPair.Value == InAbilityClass)
		{
			return true;
		}
	}

	return false;
}

void ABaseCharacter::HandleDead()
{
	OnDead();
}

void ABaseCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorPawn, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorPawn, DamageCauser);
}

void ABaseCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// We only call the BP callback if this is not the initial ability setup
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void ABaseCharacter::HandleEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnEnergyChanged(DeltaValue, EventTags);
	}
}

void ABaseCharacter::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	if (bAbilitiesInitialized)
	{
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
}

void ABaseCharacter::HandleStiffFactorChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnStiffFactorChanged(DeltaValue, EventTags);
	}
}

UBehaviorComponent* ABaseCharacter::GetBehaviorComponent() const
{
	return BehaviorComponent;
}

EBehaviorType ABaseCharacter::GetCurrentBehavior()
{
	return BehaviorComponent->GetBehavior();
}
