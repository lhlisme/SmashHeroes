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

	EWeaponMeshType WeaponMeshType = EWeaponMeshType::StaticMeshWeapon;

public:
	// ÎäÆ÷Åö×²¼ì²âÊôÐÔ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCheck")
	//FName StartPointName;		// ÎäÆ÷Åö×²¼ì²âÆðÊ¼µã²å²ÛÃû³Æ

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCheck")
	//FName EndPointName;			// ÎäÆ÷Åö×²¼ì²â½áÊøµã²å²ÛÃû³Æ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCheck")
	TArray<FName> CheckPointName;		// ÎäÆ÷Åö×²¼ì²âÆðÊ¼µã²å²ÛÃû³Æ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitCheck")
	FVector WeaponHalfSize;		// ÎäÆ÷Åö×²Ìå»ýHalfSize

public:
	virtual void ActivateWeapon();

	virtual void DeactivateWeapon();

	bool IsActivated() { return bIsActivated; }

	FORCEINLINE class UMeshComponent* GetWeaponMesh() { return Mesh; }

	EWeaponMeshType GetWeaponMeshType() { return WeaponMeshType; }

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
