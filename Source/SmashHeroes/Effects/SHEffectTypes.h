// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "SHEffectTypes.generated.h"

/**
 * 特效相关的结构定义
 */

/** 粒子特效定义 */
USTRUCT(BlueprintType)
struct FSHParticleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "粒子特效"), Category = "Particle")
	UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效是否跟随"), Category = "Particle")
	bool bParticleAttached;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "绑定Socket名称", EditCondition = "bParticleAttached"), Category = "Particle")
	FName ParticleSocketName;

	/** bParticleAttached为true时是相对位移, 否则为世界空间位置 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效位置"), Category = "Particle")
	FVector ParticleLocation;

	/** bParticleAttached为true时是相对旋转, 否则为世界空间旋转 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效旋转"), Category = "Particle")
	FRotator ParticleRotation;

	/** bParticleAttached为true时是相对缩放, 否则为世界空间缩放 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效缩放"), Category = "Particle")
	FVector ParticleScale;

};

/** 音效定义 */
USTRUCT(BlueprintType)
struct FSHSoundInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声音"), Category = "Sound")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声音大小系数"), Category = "Sound")
	float VolumeMultiplier;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声音范围系数"), Category = "Sound")
	float PitchMultiplier;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声音是否跟随"), Category = "Sound")
	bool bSoundAttached;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "绑定Socket名称", EditCondition = "bSoundAttached"), Category = "Sound")
	FName SoundSocketName;

};
