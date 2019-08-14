// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotifyState_WeaponTrail.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectHash.h"
#include "ParticleEmitterInstances.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/TypeData/ParticleModuleTypeDataAnimTrail.h"
#include "Animation/AnimInstance.h"
#include "Characters/BaseCharacter.h"

typedef TInlineComponentArray<UParticleSystemComponent*, 8> SHParticleSystemComponentArray;

/////////////////////////////////////////////////////
// UAnimNotifyState_Trail

USHAnimNotifyState_WeaponTrail::USHAnimNotifyState_WeaponTrail(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LeftPSTemplate = NULL;
	RightPSTemplate = NULL;
	FirstSocketName = NAME_None;
	SecondSocketName = NAME_None;
	WidthScaleMode = ETrailWidthMode_FromCentre;
	WidthScaleCurve = NAME_None;

	bRecycleSpawnedSystems = true;
}

float USHAnimNotifyState_WeaponTrail::GetCurveWidth(USkeletalMeshComponent* MeshComp) const
{
	UAnimInstance* AnimInst = MeshComp->GetAnimInstance();
	float Width = 1.0f;
	if (WidthScaleCurve != NAME_None && AnimInst)
	{
		if (!AnimInst->GetCurveValue(WidthScaleCurve, Width))
		{
			// Fallback to 1.f if curve was not found
			Width = 1.f;
		}
	}
	return Width;
}

void USHAnimNotifyState_WeaponTrail::StartWeaponTrail(USkeletalMeshComponent* MeshComp, AWeapon* Weapon, UParticleSystem* PSTemplate)
{
	if (!Weapon || !PSTemplate)
	{
		return;
	}

	SHParticleSystemComponentArray Children;
	Weapon->GetComponents(Children);

	float Width = GetCurveWidth(MeshComp);

	UParticleSystemComponent* RecycleCandidates[3] = { nullptr, nullptr, nullptr }; // in order of priority
	bool bFoundExistingTrail = false;
	for (UParticleSystemComponent* ParticleComp : Children)
	{
		if (ParticleComp->IsActive())
		{
			UParticleSystemComponent::TrailEmitterArray TrailEmitters;
			ParticleComp->GetOwnedTrailEmitters(TrailEmitters, this, false);

			if (TrailEmitters.Num() > 0)
			{
				// This has active emitters, we'll just restart this one.
				bFoundExistingTrail = true;

				//If there are any trails, ensure the template hasn't been changed. Also destroy the component if there are errors.
				if (PSTemplate != ParticleComp->Template && ParticleComp->GetOuter() == Weapon->GetWeaponMesh())
				{
					//The PSTemplate was changed so we need to destroy this system and create it again with the new template. May be able to just change the template?
					ParticleComp->DestroyComponent();
				}
				else
				{
					for (FParticleAnimTrailEmitterInstance* Trail : TrailEmitters)
					{
						Trail->BeginTrail();
						Trail->SetTrailSourceData(FirstSocketName, SecondSocketName, WidthScaleMode, Width);
					}
				}

				break;
			}
		}
		else if (ParticleComp->bAllowRecycling && !ParticleComp->IsActive())
		{
			// We prefer to recycle one with a matching template, and prefer one created by us.
			// 0: matching template, owned by mesh
			// 1: matching template, owned by actor
			// 2: non-matching template, owned by actor or mesh
			int32 RecycleIndex = 2;
			if (ParticleComp->Template == PSTemplate)
			{
				RecycleIndex = (ParticleComp->GetOuter() == Weapon->GetWeaponMesh() ? 0 : 1);
			}
			RecycleCandidates[RecycleIndex] = ParticleComp;
		}
	}

	if (!bFoundExistingTrail)
	{
		// Spawn a new component from PSTemplate, or recycle an old one.
		UParticleSystemComponent* RecycleComponent = (RecycleCandidates[0] ? RecycleCandidates[0] : (RecycleCandidates[1] ? RecycleCandidates[1] : RecycleCandidates[2]));
		UParticleSystemComponent* NewParticleComp = (RecycleComponent ? RecycleComponent : NewObject<UParticleSystemComponent>(Weapon->GetWeaponMesh()));
		NewParticleComp->bAutoDestroy = (RecycleComponent ? false : !bRecycleSpawnedSystems);
		NewParticleComp->bAllowRecycling = true;
		NewParticleComp->SecondsBeforeInactive = 0.0f;
		NewParticleComp->bAutoActivate = false;
		NewParticleComp->bOverrideLODMethod = false;
		NewParticleComp->RelativeScale3D = FVector(1.f);
		NewParticleComp->bAutoManageAttachment = true; // Let it detach when finished (only happens if not auto-destroying)
		// 附加到武器上
		NewParticleComp->SetAutoAttachParams(Weapon->GetWeaponMesh(), NAME_None);

		// When recycling we can avoid setting the template if set already.
		if (NewParticleComp->Template != PSTemplate)
		{
			NewParticleComp->SetTemplate(PSTemplate);
		}

		// Recycled components are usually already registered
		if (!NewParticleComp->IsRegistered())
		{
			NewParticleComp->RegisterComponentWithWorld(MeshComp->GetWorld());
		}

		// 附加到武器上
		NewParticleComp->AttachToComponent(Weapon->GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		NewParticleComp->ActivateSystem(true);

		UParticleSystemComponent::TrailEmitterArray TrailEmitters;
		NewParticleComp->GetOwnedTrailEmitters(TrailEmitters, this, true);

		for (FParticleAnimTrailEmitterInstance* Trail : TrailEmitters)
		{
			Trail->BeginTrail();
			Trail->SetTrailSourceData(FirstSocketName, SecondSocketName, WidthScaleMode, Width);
		}
	}
}

void USHAnimNotifyState_WeaponTrail::UpdateWeaponTrail(USkeletalMeshComponent* MeshComp, AWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	SHParticleSystemComponentArray Children;
	Weapon->GetComponents(Children);

	float Width = GetCurveWidth(MeshComp);

	for (UParticleSystemComponent* ParticleComp : Children)
	{
		if (ParticleComp->IsActive())
		{
			UParticleSystemComponent::TrailEmitterArray TrailEmitters;
			ParticleComp->GetOwnedTrailEmitters(TrailEmitters, this, false);
			for (FParticleAnimTrailEmitterInstance* Trail : TrailEmitters)
			{
				Trail->SetTrailSourceData(FirstSocketName, SecondSocketName, WidthScaleMode, Width);
			}
		}
	}
}

void USHAnimNotifyState_WeaponTrail::EndWeaponTrail(AWeapon* Weapon)
{
	if (!Weapon)
	{
		return;
	}

	SHParticleSystemComponentArray Children;
	Weapon->GetComponents(Children);

	for (UParticleSystemComponent* ParticleComp : Children)
	{
		if (ParticleComp->IsActive())
		{
			UParticleSystemComponent::TrailEmitterArray TrailEmitters;
			ParticleComp->GetOwnedTrailEmitters(TrailEmitters, this, false);
			for (FParticleAnimTrailEmitterInstance* Trail : TrailEmitters)
			{
				Trail->EndTrail();
			}
		}
	}
}

void USHAnimNotifyState_WeaponTrail::NotifyBegin(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	if (!LeftPSTemplate && !RightPSTemplate)
	{
		return;
	}

	AWeapon* LeftWeapon = nullptr;
	AWeapon* RightWeapon = nullptr;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(Owner))
		{
			LeftWeapon = OwnerCharacter->LeftWeapon;
			RightWeapon = OwnerCharacter->RightWeapon;
		}
	}

	if (bEnableLeftTrail)
	{
		StartWeaponTrail(MeshComp, LeftWeapon, LeftPSTemplate);
	}

	if (bEnableRightTrail)
	{
		StartWeaponTrail(MeshComp, RightWeapon, RightPSTemplate);
	}

	Received_NotifyBegin(MeshComp, Animation, TotalDuration);
}

void USHAnimNotifyState_WeaponTrail::NotifyTick(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	AWeapon* LeftWeapon = nullptr;
	AWeapon* RightWeapon = nullptr;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(Owner))
		{
			LeftWeapon = OwnerCharacter->LeftWeapon;
			RightWeapon = OwnerCharacter->RightWeapon;
		}
	}

	if (bEnableLeftTrail)
	{
		UpdateWeaponTrail(MeshComp, LeftWeapon);
	}

	if (bEnableRightTrail)
	{
		UpdateWeaponTrail(MeshComp, RightWeapon);
	}

	Received_NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void USHAnimNotifyState_WeaponTrail::NotifyEnd(class USkeletalMeshComponent * MeshComp, class UAnimSequenceBase * Animation)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	AWeapon* LeftWeapon = nullptr;
	AWeapon* RightWeapon = nullptr;
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(Owner))
		{
			LeftWeapon = OwnerCharacter->LeftWeapon;
			RightWeapon = OwnerCharacter->RightWeapon;
		}
	}

	if (bEnableLeftTrail)
	{
		EndWeaponTrail(LeftWeapon);
	}

	if (bEnableRightTrail)
	{
		EndWeaponTrail(RightWeapon);
	}

	Received_NotifyEnd(MeshComp, Animation);
}
