// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Templates/SubclassOf.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Abilities/SHAbilityTypes.h"
#include "SHBlueprintFunctionLibrary.h"
#include "SHProjectile.generated.h"


UCLASS(Blueprintable)
class SMASHHEROES_API ASHProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/** 投射物移动组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
	UProjectileMovementComponent* ProjectileMovement;

	/** 投射物碰撞组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
	USphereComponent* Sphere;

	/** 投射物碰撞体方向 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Projectile)
	UArrowComponent* Arrow;

	/** 是否可穿透目标 */
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	bool CanPenetrate = true;*/

	/** 投射物对应的GameplayEffectContainerSpec */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (DisplayName = "EffectContainerSpec", ExposeOnSpawn = true), Category = GameplayEffectContainer)
	FSHGameplayEffectContainerSpec EffectContainerSpec;

protected:
	/** 投射物击中的目标 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HitCheck")
	TArray<AActor*> HitActors;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 碰撞处理
	UFUNCTION(BlueprintCallable)
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Sets default values for this actor's properties
	ASHProjectile();

};
