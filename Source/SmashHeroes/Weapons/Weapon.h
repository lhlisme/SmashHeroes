// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Effects/SHEffectTypes.h"
#include "Weapons/SHHitCheckTypes.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponMeshType : uint8
{
	StaticMeshWeapon			UMETA(DisplayName = "StaticMeshWeapon"),
	SkeletalMeshWeapon			UMETA(DisplayName = "SkeletalMeshWeapon")
};

UCLASS(abstract)
class SMASHHEROES_API AWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	bool bIsActivated = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EWeaponMeshType WeaponMeshType = EWeaponMeshType::StaticMeshWeapon;

public:
	// 武器碰撞检测属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCheck")
	FSHHitCheckInfo HitCheckInfo;

	/** 武器击中材质特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect")
	FSHSurfaceHitEffects SurfaceHitEffects;

public:
	virtual void InitialWeapon();

	virtual void ActivateWeapon();

	virtual void DeactivateWeapon();

	bool IsActivated() { return bIsActivated; }

	FORCEINLINE class UMeshComponent* GetWeaponMesh() { return Mesh; }

	EWeaponMeshType GetWeaponMeshType() { return WeaponMeshType; }

	/** 根据名称获取Socket Transform */
	FTransform GetSocketTransformByName(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const;
};

UCLASS()
class SMASHHEROES_API AStaticWeapon : public AWeapon
{
	GENERATED_UCLASS_BODY()

public:
	virtual void ActivateWeapon() override;
	virtual void DeactivateWeapon() override;
};

UCLASS()
class SMASHHEROES_API ASkeletalWeapon : public AWeapon
{
	GENERATED_UCLASS_BODY()

public:
	virtual void ActivateWeapon() override;
	virtual void DeactivateWeapon() override;
};
