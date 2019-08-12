// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotify_EvadeEffect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

USHAnimNotify_EvadeEffect::USHAnimNotify_EvadeEffect()
{
}

void USHAnimNotify_EvadeEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// 从脚底发出射线追踪, 判断地面物理材质
	FVector TraceStart = MeshComp->GetComponentLocation();
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 150.0f);
	FHitResult HitResult;

	bool bSuccess = UKismetSystemLibrary::LineTraceSingle(MeshComp, TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_Pawn), false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);
	if (bSuccess && HitResult.PhysMaterial.IsValid())
	{
		if (const FSHEffectInfo* EffectInfo = EvadeEffects.Find(HitResult.PhysMaterial->SurfaceType))
		{
			// TODO 粒子特效可能应该与人物Forward相反
			EffectInfo->ParticleInfo.SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
			EffectInfo->SoundInfo.SpawnSelf(MeshComp, HitResult.Location, MeshComp->GetComponentRotation());
		}
	}
}
