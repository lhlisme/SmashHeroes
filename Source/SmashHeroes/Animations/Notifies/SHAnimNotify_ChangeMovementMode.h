// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SHAnimNotify_ChangeMovementMode.generated.h"

/**
 * 更改移动模式通知
 */
UCLASS(meta = (DisplayName = "ChangeMovementMode"))
class SMASHHEROES_API USHAnimNotify_ChangeMovementMode : public UAnimNotify
{
	GENERATED_BODY()

public:
	USHAnimNotify_ChangeMovementMode();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "目标移动模式"))
	TEnumAsByte<EMovementMode> TargetMovementMode;

};
