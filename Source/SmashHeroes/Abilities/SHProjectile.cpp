// Fill out your copyright notice in the Description page of Project Settings.


#include "SHProjectile.h"

// Sets default values
ASHProjectile::ASHProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建投射物碰撞体
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->InitBoxExtent(FVector(24.0f, 24.0f, 24.0f));
	BoxCollision->AreaClass = UNavArea_Obstacle::StaticClass();
	// 设置碰撞预设, 只检测Pawn的碰撞
	BoxCollision->SetCollisionProfileName(TEXT("Projectile"));
	// 设置开始碰撞事件委托
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASHProjectile::OnOverlapBegin);
	// 将碰撞体设为根组件
	RootComponent = BoxCollision;

	// 创建投射物碰撞体方向组件
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(BoxCollision);

	// 创建投射物移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(BoxCollision);
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// 设置投射物初始生命周期为1s
	InitialLifeSpan = 1.0f;
}

// Called when the game starts or when spawned
void ASHProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ASHProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 保证一个投射物只命中同一目标一次
	if (HitActors.Contains(OtherActor))
	{
		return;
	}

	// UE_LOG(LogTemp, Log, TEXT("%s  Hit  %s,  Instigator:  %s"), *GetFName().ToString(), *OtherActor->GetFName().ToString(), *GetInstigator()->GetFName().ToString());
	if (GetInstigator() != OtherActor)
	{
		// Projetile之间的碰撞相互忽略
		HitActors.Add(OtherActor);
		// 自动创建的空对象
		TArray<FHitResult> HitResults;
		FSHGameplayEffectContainerSpec NewSpec = USHBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(EffectContainerSpec, HitResults, HitActors);
		USHBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(NewSpec);

		// 如果不是可穿透投射物，在第一次有效命中时应销毁
		if (!CanPenetrate)
		{
			if (EmitterTemplate)
			{
				// 生成命中粒子特效
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, SweepResult.Location);
			}
			// 销毁投射物
			Destroy();
		}
	}
}

FVector ASHProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;

	return ProjectileMovement->Velocity;
}

