// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			for (int32 i = 0; i < CharacterAbilities.Num(); ++i)
			{
				if (!CharacterAbilities[i])
				{
					continue;
				}
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(CharacterAbilities[i].GetDefaultObject(), 1, 0));
			}

			for (int32 i = 0; i < CharacterAttributeSets.Num(); ++i)
			{
				AbilitySystem->InitStats(CharacterAttributeSets[i], nullptr);
			}
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}

	// 生成武器
	GenerateWeapon();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 如果不在攻击状态，正常更新武器插槽位置
	if (!IsAttacking) {
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

// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes.
// As pawns may be spawned before the client controller possesses them especially in a multiplayer enviroment.
void ABaseCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->RefreshAbilityActorInfo();
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

bool ABaseCharacter::Attack()
{
	// 返回值表示是否有效执行
	return false;
}

void ABaseCharacter::BeginAttack()
{
	IsAttacking = true;
}

void ABaseCharacter::EndAttack()
{
	IsAttacking = false;
}

UAnimMontage* ABaseCharacter::GetAttackMontageByIndex()
{
	UAnimMontage** CurAttackMontage = AttackMontageMap.Find(ComboIndex);

	return *CurAttackMontage;
}

bool ABaseCharacter::Evade()
{
	// 返回值表示是否有效执行
	return false;
}

void ABaseCharacter::BeginEvade()
{
	IsEvading = true;
}

void ABaseCharacter::EndEvade()
{
	IsEvading = false;
}

bool ABaseCharacter::Guard()
{
	// 返回值表示是否有效执行
	return false;
}

void ABaseCharacter::BeginGuard()
{
	IsGuarding = true;
}

void ABaseCharacter::EndGuard()
{
	IsGuarding = false;
}

bool ABaseCharacter::AttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits)
{
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
					}
				}

				// 攻击检测结束后更新武器指定插槽位置
				RightWeapon->UpdateSocketLocation(i, EndLocation);
			}
		}
	}


	return true;
}

