// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotify_FootEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USHAnimNotify_FootEffect::USHAnimNotify_FootEffect()
{
}

void USHAnimNotify_FootEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// �ӽŵ׷�������׷��, �жϵ����������
	FVector TraceStart;
	if (FName* SocketName = SocketNames.Find(FootType))
	{
		TraceStart = MeshComp->GetSocketLocation(*SocketName);
	}
	else
	{
		TraceStart = MeshComp->GetComponentLocation();
	}

	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 100.0f);
	FHitResult HitResult;

	bool bSuccess = UKismetSystemLibrary::LineTraceSingle(MeshComp, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Pawn), false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);
	if (bSuccess && HitResult.PhysMaterial.IsValid())
	{
		if (const FSHParticleInfo* ParticleInfo = Particles.Find(HitResult.PhysMaterial->SurfaceType))
		{
			// TODO ������Ч����Ӧ��������Forward�෴
			ParticleInfo->SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
		}

		if (const FSHSoundInfo* SoundInfo = Sounds.Find(HitResult.PhysMaterial->SurfaceType))
		{
			SoundInfo->SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
		}
	}
}