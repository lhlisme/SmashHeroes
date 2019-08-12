// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Effects/SHEffectTypes.h"
#include "SHAnimNotify_EvadeEffect.generated.h"

/**
 * 翻滚特效通知
 */
UCLASS(meta = (DisplayName = "EvadeEffect"))
class SMASHHEROES_API USHAnimNotify_EvadeEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	USHAnimNotify_EvadeEffect();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Evade特效"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHEffectInfo> EvadeEffects;
};
