// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotify_PutWeapon.h"


USHAnimNotify_PutWeapon::USHAnimNotify_PutWeapon()
{
}

void USHAnimNotify_PutWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TArray<USceneComponent*> ChildComps = MeshComp->GetAttachChildren();

	for (auto& CurComp : ChildComps)
	{
		if (CurComp->GetFName() == WeaponName)
		{
			CurComp->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform, TargetSocketName);
			break;
		}
	}
}

