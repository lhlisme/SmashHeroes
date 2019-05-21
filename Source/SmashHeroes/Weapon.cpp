// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

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

