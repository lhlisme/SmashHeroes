// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Effects/SHEffectTypes.h"
#include "SHAnimNotify_HitEffect.generated.h"

/**
 * 受击特效通知
 */
UCLASS(meta = (DisplayName = "HitEffect"))
class SMASHHEROES_API USHAnimNotify_HitEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	USHAnimNotify_HitEffect();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Hit粒子特效"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHParticleInfo> Particles;
};
