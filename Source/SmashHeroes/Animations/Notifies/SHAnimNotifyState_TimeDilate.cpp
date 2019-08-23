// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotifyState_TimeDilate.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BaseCharacter.h"

USHAnimNotifyState_TimeDilate::USHAnimNotifyState_TimeDilate()
{
	bCanTimeDilate = true;
	DilationFactor = 0.2f;
}

void USHAnimNotifyState_TimeDilate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!bCanTimeDilate)
	{
		return;
	}

	if (MeshComp && MeshComp->GetWorld())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), DilationFactor);
			Character->CustomTimeDilation /= DilationFactor;
		}
	}
}

void USHAnimNotifyState_TimeDilate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!bCanTimeDilate)
	{
		return;
	}

	if (MeshComp && MeshComp->GetWorld())
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner()))
		{
			UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.0f);
			Character->CustomTimeDilation = 1.0f;
		}
	}
}
