// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SHAnimNotifyState_TimeDilate.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHAnimNotifyState_TimeDilate : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	/** ʱ������Ч������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	bool bCanTimeDilate;
	/** ʱ���������� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Dilation")
	float DilationFactor;

public:
	USHAnimNotifyState_TimeDilate();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
