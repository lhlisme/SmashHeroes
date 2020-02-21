// Fill out your copyright notice in the Description page of Project Settings.


#include "SHEffectTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"


UParticleSystemComponent* FSHParticleInfo::SpawnSelf(UMeshComponent* OwnerMeshComp, FVector TargetLocation, FRotator TargetRotation, FVector TargetScale) const
{
	if (!OwnerMeshComp || !Particle)
	{
		return nullptr;
	}
	
	UParticleSystemComponent* PSC = nullptr;
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

UAudioComponent* FSHSoundInfo::SpawnSelf(UMeshComponent* OwnerMeshComp, FVector TargetLocation, FRotator TargetRotation) const
{
	if (!OwnerMeshComp || !Sound)
	{
		return nullptr;
	}

	UAudioComponent* AC = nullptr;
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

void FSHSurfaceHitEffects::PlayHitEffect(const FHitResult& HitResult, EAttackStrength AttackStrength, UMeshComponent* ActorMesh) const
{
	if (HitResult.PhysMaterial.IsValid() && ActorMesh)
	{
		if (const FSHHitEffects* HitEffect = HitEffectMap.Find(HitResult.PhysMaterial->SurfaceType))
		{
			if (const FSHEffectInfo* EffectInfo = HitEffect->EffectInfoMap.Find(AttackStrength))
			{
				EffectInfo->ParticleInfo.SpawnSelf(ActorMesh, HitResult.Location, ActorMesh->GetComponentRotation());
				EffectInfo->SoundInfo.SpawnSelf(ActorMesh, HitResult.Location, ActorMesh->GetComponentRotation());
			}
		}
	}
}

