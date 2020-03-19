// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SHAnimNotify_PutWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHAnimNotify_PutWeapon : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	USHAnimNotify_PutWeapon();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "源端Socket名称"))
	FName SourceSocketName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "目的Socket名称"))
	FName TargetSocketName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "武器名称"))
	FName WeaponName;
};
