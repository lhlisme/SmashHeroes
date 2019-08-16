// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBox.h"

// Sets default values
ASpawnBox::ASpawnBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->InitBoxExtent(FVector(64.0f, 64.0f, 64.0f));
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetGenerateOverlapEvents(false);
	SetRootComponent(Box);
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->AttachTo(RootComponent);
	Billboard->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	// 广告牌只在编辑器版本使用
	Billboard->bIsEditorOnly = true;
}

// Called when the game starts or when spawned
void ASpawnBox::BeginPlay()
{
	Super::BeginPlay();
	
}

