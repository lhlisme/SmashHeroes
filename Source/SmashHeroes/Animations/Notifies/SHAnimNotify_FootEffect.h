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
 * �Ų���Ч����Ч֪ͨ
 */
UCLASS(meta = (DisplayName = "FootEffect"))
class SMASHHEROES_API USHAnimNotify_FootEffect : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	USHAnimNotify_FootEffect();

	virtual void Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation) override;

public:
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot����"))
	EFootType FootType;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Foot����"))
	TMap<EFootType, FName> SocketNames;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot������Ч"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHParticleInfo> Particles;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Foot��Ч"))
	TMap<TEnumAsByte<EPhysicalSurface>, FSHSoundInfo> Sounds;
};
