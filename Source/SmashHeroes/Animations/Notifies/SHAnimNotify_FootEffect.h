// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Effects/SHEffectTypes.h"
#include "SHAnimNotify_FootEffect.generated.h"

UENUM(BlueprintType)
enum class EFootType : uint8
{
	LeftFoot			UMETA(DisplayName = "MeleeAttack"),
	RightFoot			UMETA(DisplayName = "RangeAttack")
};

/**
 * 脚步音效、特效通知
 */
UCLASS(meta = (DisplayName = "FootEffect"))
class SMASHHEROES_API USHAnimNotify_FootEffect : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	USHAnimNotify_FootEffect();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot类型"))
	EFootType FootType;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Foot骨骼"))
	TMap<EFootType, FName> SocketNames;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot粒子特效"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHParticleInfo> Particles;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot音效"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHSoundInfo> Sounds;
};
