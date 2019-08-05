// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Abilities/SHGameplayAbility.h"

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

	// 如果不在近战攻击状态，正常更新武器插槽位置
	if (BehaviorComponent->GetBehavior() != EBehaviorType::MeleeAttack) {
		if (LeftWeapon) {
			LeftWeapon->UpdateSocketLocations();
		}
		if (RightWeapon) {
			RightWeapon->UpdateSocketLocations();
		}
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
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftWeaponSocket);
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightWeaponSocket);
			LeftWeapon->InitialWeapon();
			RightWeapon->InitialWeapon();
		}
		break;
	}
	case EArmedState::LeftHold: {
		LeftWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (LeftWeapon) {
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftWeaponSocket);
			LeftWeapon->InitialWeapon();
		}
		break;
	}
	case EArmedState::RightHold: {
		RightWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (RightWeapon) {
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightWeaponSocket);
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

// 已存在返回false, 否则返回true
bool ABaseCharacter::AddLeftDamagedActor(AActor* CurDamagedActor)
{
	if (LeftDamagedActors.Contains(CurDamagedActor)) {
		int32* CurValue = LeftDamagedActors.Find(CurDamagedActor);
		++(*CurValue);	// 出现次数增加
		return false;
	}
	LeftDamagedActors.Add(CurDamagedActor, 1);
	return true;
}

// 已存在返回false, 否则返回true
bool ABaseCharacter::AddRightDamagedActor(AActor* CurDamagedActor)
{
	if (RightDamagedActors.Contains(CurDamagedActor)) {
		int32* CurValue = RightDamagedActors.Find(CurDamagedActor);
		++(*CurValue);	// 出现次数增加
		return false;
	}
	RightDamagedActors.Add(CurDamagedActor, 1);
	return true;
}

void ABaseCharacter::ClearDamagedActors()
{
	LeftDamagedActors.Empty();
	RightDamagedActors.Empty();
}

bool ABaseCharacter::MeleeAttack()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetMeleeAttackMontageByIndex()
{
	UAnimMontage** CurAttackMontagePtr = MeleeAttackMontageMap.Find(AttackIndex);

	if (CurAttackMontagePtr) {
		return *CurAttackMontagePtr;
	}

	return nullptr;
}

bool ABaseCharacter::RangeAttack()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetRangeAttackMontageByIndex()
{
	UAnimMontage** CurAttackMontagePtr = RangeAttackMontageMap.Find(AttackIndex);

	if (CurAttackMontagePtr) {
		return *CurAttackMontagePtr;
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

		// 若攻击被格挡, 仍处于防御状态, 否则进入受击状态
		if (FirstHitReaction != EHitReaction::BlockHit)
		{
			// 进入受击状态
			BehaviorComponent->BeginHit();
		}

		if (HitMontagePtr)
		{
			PlayAnimMontage(*HitMontagePtr);
		}

		// 更新仇恨值
		UpdateHateValue(DamageAmount, DamageCauser);

		// 播放受击特效
		PlayHitEffect(InLinearColor);
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

void ABaseCharacter::PushHitReaction(EHitReaction NewHitReaction)
{
	HitReactions.Push(NewHitReaction);
}

EHitReaction ABaseCharacter::PopHitReaction()
{
	// 弹出队首的HitReaction并返回
	EHitReaction FirstHitReaction = EHitReaction::HitFront;

	if (HitReactions.Num() > 0)
	{
		FirstHitReaction = HitReactions[0];
		HitReactions.RemoveAt(0);
	}

	return FirstHitReaction;
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

bool ABaseCharacter::IsHitInDefenseRange(FVector HitLocation, EHitReaction& HitReaction)
{
	bool bHitInDefenseRange = false;
	// 受击点相对当前人物的旋转
	FRotator DeltaRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitLocation) - GetActorRotation();

	// 如果受击点处于防御范围内
	if (DeltaRotator.Yaw > -0.5f * GetDefenseRange() && DeltaRotator.Yaw <= 0.5f * GetDefenseRange())
	{
		bHitInDefenseRange = true;
		HitReaction = EHitReaction::BlockHit;
	}

	UE_LOG(LogTemp, Log, TEXT("DeltaRotator: %f"), DeltaRotator.Yaw);
	// 如果受击点不在防御范围内, 则未成功格挡攻击
	if (!bHitInDefenseRange)
	{
		if (DeltaRotator.Yaw <= 45.0f)
		{
			if (DeltaRotator.Yaw > -45.0f)
			{
				HitReaction = EHitReaction::HitFront;
			}
			else if (DeltaRotator.Yaw > -135.0f)
			{
				HitReaction = EHitReaction::HitLeft;
			}
			else
			{
				HitReaction = EHitReaction::HitBack;
			}
		}
		else
		{
			if (DeltaRotator.Yaw <= 135.0f)
			{
				HitReaction = EHitReaction::HitRight;
			}
			else
			{
				HitReaction = EHitReaction::HitBack;
			}
		}
	}

	return bHitInDefenseRange;
}

void ABaseCharacter::CheckHitResult(FVector HitLocation, float DefenseFactor, float& DamageDone, float& EnergyCost)
{
	EHitReaction HitReaction = EHitReaction::HitFront;
	bool bIsBlocked = false;	// 本次攻击是否被成功格挡

	if (BehaviorComponent->GetBehavior() == EBehaviorType::Guard)
	{
		float ScaleFactor = 0.5f;
		float RestEnergy = GetEnergy();		// 剩余能量值
		float MaxHealth = GetMaxHealth();
		float MaxEnergy = GetMaxEnergy();

		// 受击点在防御范围内
		if (IsHitInDefenseRange(HitLocation, HitReaction))
		{
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
	}

	PushHitReaction(HitReaction);
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

bool ABaseCharacter::Guard()
{
	// 返回值表示是否有效执行
	return false;
}

UAnimMontage* ABaseCharacter::GetGuardMontage()
{
	return GuardMontage;
}

bool ABaseCharacter::MeleeAttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData)
{
	bool Hitted = false;	// 是否命中目标
	// 近战攻击检测
	if (AttackType == EAttackType::MeleeAttack) {
		if (LeftWeapon) {
			for (int32 i = 0; i < LeftWeapon->SocketLocations.Num(); ++i) {
				FVector StartLocation = LeftWeapon->SocketLocations[i];
				FVector EndLocation = LeftWeapon->GetCurrentSocketLocation(i);
				TArray<FHitResult> OutHits;

				UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, TraceHitColor, DrawTime);
				
				for (int32 j = 0; j < OutHits.Num(); ++j) {
					if (AddLeftDamagedActor(OutHits[j].GetActor())) {	// 添加成功(不存在)时返回true
						FinalOutHits.Add(OutHits[j]);
						FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(OutHits[j]);
						HitTargetsData.Add(NewData);
						Hitted = true;
					}
				}

				// 攻击检测结束后更新武器指定插槽位置
				LeftWeapon->UpdateSocketLocation(i, EndLocation);
			}
		}

		if (RightWeapon) {
			for (int32 i = 0; i < RightWeapon->SocketLocations.Num(); ++i) {
				FVector StartLocation = RightWeapon->SocketLocations[i];
				FVector EndLocation = RightWeapon->GetCurrentSocketLocation(i);
				TArray<FHitResult> OutHits;

				UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), StartLocation, EndLocation, ObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, TraceHitColor, DrawTime);

				for (int32 j = 0; j < OutHits.Num(); ++j) {
					if (AddRightDamagedActor(OutHits[j].GetActor())) {	// 添加成功(不存在)时返回true
						FinalOutHits.Add(OutHits[j]);
						FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(OutHits[j]);
						HitTargetsData.Add(NewData);
						Hitted = true;
					}
				}

				// 攻击检测结束后更新武器指定插槽位置
				RightWeapon->UpdateSocketLocation(i, EndLocation);
			}
		}
	}

	return Hitted;
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

void ABaseCharacter::PlayHitEffect(FLinearColor InLinearColor)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	if (CharacterMesh)
	{
		CharacterMesh->SetVectorParameterValueOnMaterials(FName(TEXT("EffectColor")), FVector(InLinearColor));
		CharacterMesh->SetScalarParameterValueOnMaterials(FName(TEXT("StartTime")), UGameplayStatics::GetTimeSeconds(GetWorld()));
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

void ABaseCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystem);

	if (Role == ROLE_Authority && !bAbilitiesInitialized) {
		// 仅在服务器端发授能力
		for (TSubclassOf<USHGameplayAbility>& StartupAbility : CharacterAbilities) {
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
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
			if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass())) {
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

UBehaviorComponent* ABaseCharacter::GetBehaviorComponent()
{
	return BehaviorComponent;
}

