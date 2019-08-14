// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "UObject/ObjectMacros.h"
#include "Engine/EngineTypes.h"
#include "Weapons/Weapon.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SHAnimNotifyState_WeaponTrail.generated.h"

class UParticleSystem;
class UParticleSystemComponent;
class USkeletalMeshComponent;
class UAnimSequenceBase;

/**
 * 武器拖尾效果通知状态
 */
UCLASS(editinlinenew, Blueprintable, const, hidecategories = Object, collapsecategories, meta = (DisplayName = "WeaponTrail"))
class SMASHHEROES_API USHAnimNotifyState_WeaponTrail : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

public:
	/** 开启左手武器拖尾 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	bool bEnableLeftTrail;

	/** 开启右手武器拖尾 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	bool bEnableRightTrail;

	/** 左手武器拖尾 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	UParticleSystem* LeftPSTemplate;

	/** 右手武器拖尾 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	UParticleSystem* RightPSTemplate;

	/** Name of the first socket defining this trail. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	FName FirstSocketName;

	/** Name of the second socket defining this trail. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	FName SecondSocketName;

	/**
	Controls the way width scale is applied. In each method a width scale of 1.0 will mean the width is unchanged from the position of the sockets. A width scale of 0.0 will cause a trail of zero width.
	From Centre = Trail width is scaled outwards from the centre point between the two sockets.
	From First = Trail width is scaled outwards from the position of the first socket.
	From Second = Trail width is scaled outwards from the position of the Second socket.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	TEnumAsByte<enum ETrailWidthMode> WidthScaleMode;

	/** Name of the curve to drive the width scale. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponTrail")
	FName WidthScaleCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponTrail")
	uint32 bRecycleSpawnedSystems : 1;

protected:
	void StartWeaponTrail(USkeletalMeshComponent* MeshComp, AWeapon* Weapon, UParticleSystem* PSTemplate);
	void UpdateWeaponTrail(USkeletalMeshComponent* MeshComp, AWeapon* Weapon);
	void EndWeaponTrail(AWeapon* Weapon);

public:
	virtual float GetCurveWidth(USkeletalMeshComponent* MeshComp) const;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
