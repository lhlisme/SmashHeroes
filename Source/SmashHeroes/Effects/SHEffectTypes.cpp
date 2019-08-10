// Fill out your copyright notice in the Description page of Project Settings.


#include "SHEffectTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"


UParticleSystemComponent* FSHParticleInfo::SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation, FRotator TargetRotation, FVector TargetScale) const
{
	if (!OwnerMeshComp || !Particle)
	{
		return nullptr;
	}
	
	UParticleSystemComponent* PSC = nullptr;
	// TODO 特效多人联机时同步
	if (bParticleAttached)
	{
		PSC = UGameplayStatics::SpawnEmitterAttached(Particle, OwnerMeshComp, ParticleSocketName, RelativeLocation, RelativeRotation);
	}
	else 
	{
		FTransform SpawnTransform(TargetRotation, TargetLocation, TargetScale);
		PSC = UGameplayStatics::SpawnEmitterAtLocation(OwnerMeshComp->GetWorld(), Particle, SpawnTransform);
	}

	return PSC;
}

UAudioComponent* FSHSoundInfo::SpawnSelf(USkeletalMeshComponent* OwnerMeshComp, FVector TargetLocation, FRotator TargetRotation) const
{
	if (!OwnerMeshComp || !Sound)
	{
		return nullptr;
	}

	UAudioComponent* AC = nullptr;
	// TODO 声音多人联机时同步
	if (bSoundAttached)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, OwnerMeshComp, SoundSocketName, RelativeLocation, RelativeRotation, EAttachLocation::KeepRelativeOffset, false, VolumeMultiplier, PitchMultiplier, 0.0f);
	}
	else
	{
		AC = UGameplayStatics::SpawnSoundAtLocation(OwnerMeshComp->GetWorld(), Sound, TargetLocation, TargetRotation, VolumeMultiplier, PitchMultiplier, 0.0f);
	}

	return AC;
}