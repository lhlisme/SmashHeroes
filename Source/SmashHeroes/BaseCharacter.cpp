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
		}
		break;
	}
	case EArmedState::LeftHold: {
		LeftWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (LeftWeapon) {
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, LeftWeaponSocket);
		}
		break;
	}
	case EArmedState::RightHold: {
		RightWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (RightWeapon) {
			RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, RightWeaponSocket);
		}
		break;
	}
	default:
		break;
	}
}

bool ABaseCharacter::AddDamagedActor(AActor* CurDamagedActor)
{
	if (DamagedActors.Contains(CurDamagedActor)) {
		int32* CurValue = DamagedActors.Find(CurDamagedActor);
		++(*CurValue);	// 出现次数增加
		return false;
	}
	DamagedActors.Add(CurDamagedActor, 0);
	return true;
}

void ABaseCharacter::ClearDamagedActors()
{
	DamagedActors.Empty();
}

bool ABaseCharacter::AttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	// 近战攻击检测
	if (AttackType == EAttackType::MeleeAttack) {
		/*if (LeftWeapon) {
			FVector StartPoint = LeftWeapon->GetWeaponMesh()->GetSocketLocation(LeftWeapon->StartPointName);
			FVector EndPoint = LeftWeapon->GetWeaponMesh()->GetSocketLocation(LeftWeapon->EndPointName);
			UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartPoint, EndPoint, LeftWeapon->WeaponHalfSize, FRotator(0.f, 0.f, 0.f), ObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, TraceHitColor, DrawTime);
		}
		if (RightWeapon) {
			FVector StartPoint = RightWeapon->GetWeaponMesh()->GetSocketLocation(RightWeapon->StartPointName);
			FVector EndPoint = RightWeapon->GetWeaponMesh()->GetSocketLocation(RightWeapon->EndPointName);
			UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), StartPoint, EndPoint, RightWeapon->WeaponHalfSize, FRotator(0.f, 0.f, 0.f), ObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, TraceHitColor, DrawTime);
		}*/
	}
	return true;
}

