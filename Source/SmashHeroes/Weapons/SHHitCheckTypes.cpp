// Fill out your copyright notice in the Description page of Project Settings.


#include "SHHitCheckTypes.h"
#include "Characters/BaseCharacter.h"


void FSHHitCheckInfo::SetSocketNames(TArray<FName> InSocketNames)
{
	for (int32 i = 0; i < InSocketNames.Num(); ++i)
	{
		if (!ExcludedNames.Contains(InSocketNames[i]))
		{
			SocketNames.Add(InSocketNames[i]);
		}
	}
}

FVector FSHHitCheckInfo::GetUpdatedSocketLocation(int32 SocketIndex, UMeshComponent* ActorMesh)
{
	FVector CurrentLocation = FVector::ZeroVector;
	if (ActorMesh && SocketIndex < SocketNames.Num()) {
		CurrentLocation = ActorMesh->GetSocketLocation(SocketNames[SocketIndex]);
	}

	return CurrentLocation;
}

void FSHHitCheckInfo::InitSocketLocations()
{
	int32 SocketsNum = SocketNames.Num();
	if (SocketsNum > 0)
	{
		SocketLocations.Init(FVector::ZeroVector, SocketsNum);
	}
}

void FSHHitCheckInfo::UpdateSocketLocations(UMeshComponent* ActorMesh)
{
	if (!ActorMesh)
	{
		return;
	}

	for (int32 i = 0; i < SocketNames.Num(); ++i) {
		if (SocketLocations.Num() > i) {
			SocketLocations[i] = ActorMesh->GetSocketLocation(SocketNames[i]);
		}
		else {
			SocketLocations.Add(ActorMesh->GetSocketLocation(SocketNames[i]));
		}
	}
}

bool FSHHitCheckInfo::AddDamagedActor(AActor* CurDamagedActor)
{
	if (DamagedActors.Contains(CurDamagedActor)) {
		int32* CurValue = DamagedActors.Find(CurDamagedActor);
		++(*CurValue);	// ���ִ�������

		return false;
	}
	DamagedActors.Add(CurDamagedActor, 1);

	return true;
}

void FSHHitCheckInfo::ClearDamagedActors()
{
	DamagedActors.Empty();
}

void FSHHitCheckInfo::HitCheck(ABaseCharacter* InCharacter, const EAttackStrength AttackStrength, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, UMeshComponent* ActorMesh, const FSHSurfaceHitEffects& SurfaceHitEffects, EHitReaction DefaultHitReaction, FVector InImpulse, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData, bool& IsHit)
{
	if (!InCharacter || !ActorMesh)
	{
		return;
	}

	for (int32 i = 0; i < SocketLocations.Num(); ++i)
	{
		FVector StartLocation = SocketLocations[i];
		FVector EndLocation = GetUpdatedSocketLocation(i, ActorMesh);
		TArray<FHitResult> OutHits;

		UKismetSystemLibrary::LineTraceMultiForObjects(InCharacter->GetWorld(), StartLocation, EndLocation, ObjectTypes, true, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, TraceHitColor, DrawTime);

		AActor* HitActor = nullptr;
		ABaseCharacter* HitCharacter = nullptr;
		for (int32 j = 0; j < OutHits.Num(); ++j)
		{
			HitActor = OutHits[j].GetActor();
			HitCharacter = Cast<ABaseCharacter>(HitActor);
			// �ж��Ƿ�Ϊ�ж�Ŀ��, ���Ƿ�Ϊ���ι����е�һ�����и�Ŀ��
			if (InCharacter->IsTargetHostile(HitActor) && AddDamagedActor(HitActor))
			{	// ��ӳɹ�(������)ʱ����true
				FinalOutHits.Add(OutHits[j]);
				FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(OutHits[j]);
				HitTargetsData.Add(NewData);
				IsHit = true;
				if (HitCharacter)
				{
					// ���Ĭ�ϻ��з���
					HitCharacter->PushDefaultHitReaction(DefaultHitReaction);
					// ʩ�ӳ����
					if (UCharacterMovementComponent* CharacterMovement = HitCharacter->GetCharacterMovement())
					{
						CharacterMovement->AddImpulse(InImpulse, true);
					}
				}
				// ����������Ч
				SurfaceHitEffects.PlayHitEffect(OutHits[j], AttackStrength, ActorMesh);
			}
		}

		// �������������������ָ�����λ��
		SocketLocations[i] = EndLocation;
	}
}

