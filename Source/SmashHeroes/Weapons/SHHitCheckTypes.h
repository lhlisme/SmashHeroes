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
	/** 单次检测中命中的所有Actor */
	TMap<AActor*, int32> DamagedActors;

	/** 用于命中检测的所有Socket的Location */
	TArray<FVector> SocketLocations;

public:
	/** 用于命中检测的所有Socket名称(不含ExcludedNames) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> SocketNames;

	/** 需要排除的Socket名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> ExcludedNames;

public:
	/** 构造函数 */
	FSHHitCheckInfo() {}

	/** 设置用于命中检测的Socket名称 */ 
	void SetSocketNames(TArray<FName> InSocketNames);

	/** 获取指定Socket的最新位置 */
	FVector GetUpdatedSocketLocation(int32 SocketIndex, UMeshComponent* ActorMesh);

	/** 初始化Socket位置 */
	void InitSocketLocations();

	/** 更新Socket位置 */
	void UpdateSocketLocations(UMeshComponent* ActorMesh);

	/** 添加命中的Actor信息 */
	bool AddDamagedActor(AActor* CurDamagedActor);

	/** 清空DamagedActors */
	void ClearDamagedActors();

	/** 命中检测 */
	void HitCheck(ABaseCharacter* InCharacter, const EAttackStrength AttackStrength, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, UMeshComponent* ActorMesh, const FSHSurfaceHitEffects& SurfaceHitEffects, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData, bool& IsHit);
};
