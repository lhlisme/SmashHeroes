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

	/** bParticleAttachedΪtrueʱ�����λ��, ����Ϊ����ռ�λ�� */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Чλ��"), Category = "Particle")
	FVector ParticleLocation;

	/** bParticleAttachedΪtrueʱ�������ת, ����Ϊ����ռ���ת */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч��ת"), Category = "Particle")
	FRotator ParticleRotation;

	/** bParticleAttachedΪtrueʱ���������, ����Ϊ����ռ����� */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "��Ч����"), Category = "Particle")
	FVector ParticleScale;

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

};
