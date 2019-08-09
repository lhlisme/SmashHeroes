// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "SHEffectTypes.generated.h"

/**
 * ��Ч��صĽṹ����
 */

/** ������Ч���� */
USTRUCT(BlueprintType)
struct FSHParticleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "������Ч"), Category = "Particle")
	UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч�Ƿ����"), Category = "Particle")
	bool bParticleAttached;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Socket����", EditCondition = "bParticleAttached"), Category = "Particle")
	FName ParticleSocketName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч���λ��", EditCondition = "bParticleAttached"), Category = "Particle")
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч�����ת", EditCondition = "bParticleAttached"), Category = "Particle")
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч�������", EditCondition = "bParticleAttached"), Category = "Particle")
	FVector RelativeScale;

	/** ����������Ч(TargetLocation��Ϊ����ռ�) */
	UParticleSystemComponent* SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation = FVector::ZeroVector, FRotator TargetRotation = FRotator::ZeroRotator, FVector TargetScale = FVector::OneVector) const;
};

/** ��Ч���� */
USTRUCT(BlueprintType)
struct FSHSoundInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "����"), Category = "Sound")
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "������Сϵ��"), Category = "Sound")
	float VolumeMultiplier;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "������Χϵ��"), Category = "Sound")
	float PitchMultiplier;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "�����Ƿ����"), Category = "Sound")
	bool bSoundAttached;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Socket����", EditCondition = "bSoundAttached"), Category = "Sound")
	FName SoundSocketName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Դ���λ��", EditCondition = "bSoundAttached"), Category = "Sound")
	FVector RelativeLocation;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Դ�����ת", EditCondition = "bSoundAttached"), Category = "Sound")
	FRotator RelativeRotation;

	/** ������Ч */
	UAudioComponent* SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation = FVector::ZeroVector, FRotator TargetRotation = FRotator::ZeroRotator) const;
};
