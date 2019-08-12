// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AWeapon::ActivateWeapon()
{
	bIsActivated = true;
}

void AWeapon::DeactivateWeapon()
{
	bIsActivated = false;
}

void AWeapon::InitialWeapon()
{
	SocketNames = Mesh->GetAllSocketNames();
}

void AWeapon::UpdateSocketLocations()
{
	for (int32 i = 0; i < SocketNames.Num(); ++i) {
		if (SocketLocations.Num() > i) {
			SocketLocations[i] = Mesh->GetSocketLocation(SocketNames[i]);
		}
		else {
			SocketLocations.Add(Mesh->GetSocketLocation(SocketNames[i]));
		}
	}
}

void AWeapon::UpdateSocketLocation(int32 SocketIndex, FVector CurLocation)
{
	if (SocketIndex < SocketLocations.Num()) {
		SocketLocations[SocketIndex] = CurLocation;
	}
}

FVector AWeapon::GetCurrentSocketLocation(int32 SocketIndex)
{
	FVector CurrentLocation = FVector(0.f, 0.f, 0.f);
	if (SocketIndex < SocketNames.Num()) {
		CurrentLocation = Mesh->GetSocketLocation(SocketNames[SocketIndex]);
	}

	return CurrentLocation;
}

void AWeapon::PlayHitEffect(FHitResult& HitResult, EAttackStrength AttackStrength)
{
	if (HitResult.PhysMaterial.IsValid())
	{
		if (const FSHHitEffect* HitEffect = HitEffects.Find(HitResult.PhysMaterial->SurfaceType))
		{
			if (const FSHEffectInfo* EffectInfo = HitEffect->EffectInfoMap.Find(AttackStrength))
			{
				EffectInfo->ParticleInfo.SpawnSelf(Mesh, HitResult.Location, Mesh->GetComponentRotation());
				EffectInfo->SoundInfo.SpawnSelf(Mesh, HitResult.Location, Mesh->GetComponentRotation());
			}
		}
	}
}

AStaticWeapon::AStaticWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetReceivesDecals(false);
	WeaponMeshType = EWeaponMeshType::StaticMeshWeapon;
}

void AStaticWeapon::ActivateWeapon()
{
	Super::ActivateWeapon();
	Mesh->SetHiddenInGame(true);
}

void AStaticWeapon::DeactivateWeapon()
{
	Super::DeactivateWeapon();
	Mesh->SetHiddenInGame(false);
}

ASkeletalWeapon::ASkeletalWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = Mesh;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetReceivesDecals(false);
	WeaponMeshType = EWeaponMeshType::SkeletalMeshWeapon;
}

void ASkeletalWeapon::ActivateWeapon()
{
	Super::ActivateWeapon();
	Mesh->SetHiddenInGame(true);
}

void ASkeletalWeapon::DeactivateWeapon()
{
	Super::DeactivateWeapon();
	Mesh->SetHiddenInGame(false);
}

