// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAnimNotify_ChangeMovementMode.h"
#include "Characters/BaseCharacter.h"


USHAnimNotify_ChangeMovementMode::USHAnimNotify_ChangeMovementMode()
{
}

void USHAnimNotify_ChangeMovementMode::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner()))
	{
		if (UCharacterMovementComponent* CharacterMovement = OwnerCharacter->GetCharacterMovement())
		{
			CharacterMovement->SetMovementMode(TargetMovementMode);
		}
	}
}
