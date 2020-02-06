// Fill out your copyright notice in the Description page of Project Settings.


#include "SHProjectile.h"

// Sets default values
ASHProjectile::ASHProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建投射物移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// 创建投射物碰撞体
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->InitSphereRadius(24.0f);
	Sphere->AreaClass = UNavArea_Obstacle::StaticClass();
	// 设置碰撞预设, 只检测Pawn的碰撞
	Sphere->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	// 设置开始碰撞事件委托
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASHProjectile::OnOverlapBegin);

	// 创建投射物碰撞体方向组件
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Sphere);

	// 设置投射物初始生命周期为1s
	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ASHProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASHProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HitActors.Contains(OtherActor))
	{
		return;
	}

	HitActors.Add(OtherActor);
	if (GetInstigator() != OtherActor)
	{
		TArray<AActor*> TargetActors;
		TargetActors.Add(OtherActor);
		// 自动创建的空对象
		TArray<FHitResult> HitResults;
		FSHGameplayEffectContainerSpec NewSpec = USHBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(EffectContainerSpec, HitResults, TargetActors);
		USHBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(NewSpec);
	}

}

