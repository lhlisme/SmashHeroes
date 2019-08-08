// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SHAnimNotify_HitEffect.generated.h"

/**
 * �ܻ���Ч֪ͨ
 */
UCLASS(meta = (DisplayName = "HitEffect"))
class SMASHHEROES_API USHAnimNotify_HitEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	USHAnimNotify_HitEffect();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

//public:

};
