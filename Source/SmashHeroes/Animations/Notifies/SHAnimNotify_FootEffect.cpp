// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotify_FootEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USHAnimNotify_FootEffect::USHAnimNotify_FootEffect()
{
}

void USHAnimNotify_FootEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// 从脚底发出射线追踪, 判断地面物理材质
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
		if (const FSHEffectInfo* EffectInfo = FootEffects.Find(HitResult.PhysMaterial->SurfaceType))
		{
			// TODO 粒子特效可能应该与人物Forward相反
			EffectInfo->ParticleInfo.SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
			EffectInfo->SoundInfo.SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
		}
	}
}