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

	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效相对位置", EditCondition = "bParticleAttached"), Category = "Particle")
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效相对旋转", EditCondition = "bParticleAttached"), Category = "Particle")
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "特效相对缩放", EditCondition = "bParticleAttached"), Category = "Particle")
	FVector RelativeScale;

	/** 生成粒子特效(TargetLocation等为世界空间) */
	UParticleSystemComponent* SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation = FVector::ZeroVector, FRotator TargetRotation = FRotator::ZeroRotator, FVector TargetScale = FVector::OneVector) const;
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

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声源相对位置", EditCondition = "bSoundAttached"), Category = "Sound")
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "声源相对旋转", EditCondition = "bSoundAttached"), Category = "Sound")
	FRotator RelativeRotation;

	/** 生成音效 */
	UAudioComponent* SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation = FVector::ZeroVector, FRotator TargetRotation = FRotator::ZeroRotator) const;
};
