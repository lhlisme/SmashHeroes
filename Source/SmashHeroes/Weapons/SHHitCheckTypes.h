// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Components/MeshComponent.h"
#include "Base/SHBaseTypes.h"
#include "Effects/SHEffectTypes.h"
#include "GameplayAbilityTargetTypes.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SHHitCheckTypes.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FSHHitCheckInfo
{
	GENERATED_BODY()

private:
	/** ���μ�������е�����Actor */
	TMap<AActor*, int32> DamagedActors;

	/** �������м�������Socket��Location */
	TArray<FVector> SocketLocations;

public:
	/** �������м�������Socket����(����ExcludedNames) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> SocketNames;

	/** ��Ҫ�ų���Socket���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> ExcludedNames;

public:
	/** ���캯�� */
	FSHHitCheckInfo() {}

	/** �����������м���Socket���� */ 
	void SetSocketNames(TArray<FName> InSocketNames);

	/** ��ȡָ��Socket������λ�� */
	FVector GetUpdatedSocketLocation(int32 SocketIndex, UMeshComponent* ActorMesh);

	/** ��ʼ��Socketλ�� */
	void InitSocketLocations();

	/** ����Socketλ�� */
	void UpdateSocketLocations(UMeshComponent* ActorMesh);

	/** ������е�Actor��Ϣ */
	bool AddDamagedActor(AActor* CurDamagedActor);

	/** ���DamagedActors */
	void ClearDamagedActors();

	/** ���м�� */
	void HitCheck(ABaseCharacter* InCharacter, const EAttackStrength AttackStrength, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, UMeshComponent* ActorMesh, const FSHSurfaceHitEffects& SurfaceHitEffects, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData, bool& IsHit);
};
