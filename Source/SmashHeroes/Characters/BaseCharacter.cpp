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
	BehaviorComponent->Init();
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 生成武器
	GenerateWeapon();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

bool ABaseCharacter::Evade()
{
	// 返回值表示是否有效执行
	return false;
}

bool ABaseCharacter::Guard()
{
	// 返回值表示是否有效执行
	return false;
}

bool ABaseCharacter::MeleeAttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits)
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

float ABaseCharacter::GetEnergy() const
{
	return CharacterAttributeSet->GetEnergy();
}

float ABaseCharacter::GetMaxEnergy() const
{
	return CharacterAttributeSet->GetMaxEnergy();
}

float ABaseCharacter::GetMoveSpeed() const
{
	return CharacterAttributeSet->GetMoveSpeed();
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

void ABaseCharacter::OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("On Damaged RestHealth: %f, DamageAmount: %f"), CharacterAttributeSet->GetHealth(), DamageAmount);
	if (IsAlive()) {
		// 根据受击方向确定要播放的Montage
		ERelativeOrientation RelativeOrientation = CalculateRelativeOrientation(DamageCauser);
		UAnimMontage** HitMontagePtr = HitMontageMap.Find(RelativeOrientation);
		
		if (HitMontagePtr) {
			PlayAnimMontage(*HitMontagePtr);
		}
	}
}

void ABaseCharacter::OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void ABaseCharacter::OnEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void ABaseCharacter::OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

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

ERelativeOrientation ABaseCharacter::CalculateRelativeOrientation(AActor* TargetActor)
{
	// 默认在前方
	ERelativeOrientation RelativeOrientation = ERelativeOrientation::Forward;
	
	if (TargetActor) {
		FRotator DeltaRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation()) - GetActorRotation();
		
		UE_LOG(LogTemp, Log, TEXT("DeltaRotator: %f"), DeltaRotator.Yaw);
		if (DeltaRotator.Yaw <= 45.0f) {
			if (DeltaRotator.Yaw > -45.0f) {
				RelativeOrientation = ERelativeOrientation::Forward;
			}
			else if (DeltaRotator.Yaw > -135.0f) {
				RelativeOrientation = ERelativeOrientation::Left;
			}
			else {
				RelativeOrientation = ERelativeOrientation::Backward;
			}
		}
		else {
			if (DeltaRotator.Yaw <= 135.0f) {
				RelativeOrientation = ERelativeOrientation::Right;
			}
			else {
				RelativeOrientation = ERelativeOrientation::Backward;
			}
		}
	}

	return RelativeOrientation;
}

UBehaviorComponent* ABaseCharacter::GetBehaviorComponent()
{
	return BehaviorComponent;
}

