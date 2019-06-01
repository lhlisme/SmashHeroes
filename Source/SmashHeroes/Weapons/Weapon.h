// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
	FVector WeaponHalfSize;		// 武器碰撞体积HalfSize

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> SocketNames;		// 武器网格所有Socket的名称

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FVector> SocketLocations;	// 武器上所有Socket的位置(在每帧结束时更新)

public:
	virtual void InitialWeapon();

	virtual void ActivateWeapon();

	virtual void DeactivateWeapon();

	bool IsActivated() { return bIsActivated; }

	FORCEINLINE class UMeshComponent* GetWeaponMesh() { return Mesh; }

	EWeaponMeshType GetWeaponMeshType() { return WeaponMeshType; }

	void UpdateSocketLocations();

	void UpdateSocketLocation(int32 SocketIndex, FVector CurLocation);

	FVector GetCurrentSocketLocation(int32 SocketIndex);	// 获取骨骼的当前位置信息
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
