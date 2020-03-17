// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMonsterCharacter::MeleeAttack()
{
	if (AttackIndex == 0)
	{
		CanCombo = false;
		CanSwitchCombo = false;

		if (InitMeleeAttacks.Num() <= 0)
		{
			return false;
		}

		AttackIndex = InitMeleeAttacks[FMath::RandRange(0, InitMeleeAttacks.Num() - 1)];
		// 更新当前攻击信息
		AttackInfo = AttackMontageMap.Find(AttackIndex);
		if (AttackInfo)
		{
			return true;
		}
	}
	else
	{
		if (CanCombo && (AttackInfo->NextComboIndex > 0))
		{
			// 正常连击
			AttackIndex = AttackInfo->NextComboIndex;

			if (CanSwitchCombo && (AttackInfo->SwitchComboIndex > 0))
			{
				// 连击切换
				AttackIndex = AttackInfo->SwitchComboIndex;
			}

			CanCombo = false;
			CanSwitchCombo = false;

			// 更新当前攻击信息
			AttackInfo = AttackMontageMap.Find(AttackIndex);
			if (AttackInfo)
			{
				return true;
			}
		}
	}

	return false;
}

bool AMonsterCharacter::RangeAttack()
{
	if (InitRangeAttacks.Num() > 0)
	{
		AttackIndex = InitRangeAttacks[FMath::RandRange(0, InitRangeAttacks.Num() - 1)];

		// 更新当前攻击信息
		AttackInfo = AttackMontageMap.Find(AttackIndex);
		if (AttackInfo)
		{
			return true;
		}
	}

	return false;
}

bool AMonsterCharacter::TryEndGuard()
{
	if (BehaviorComponent && (BehaviorComponent->GetTargetBehavior() != EBehaviorType::Guard))
	{
		BehaviorComponent->EndGuard();

		return true;
	}

	return false;
}

bool AMonsterCharacter::StartMeleeCombo()
{
	// 如果当前攻击命中敌人, 可继续追击
	if (bIsAttackHit)
	{
		return TryMeleeAttack();
	}

	return false;
}

bool AMonsterCharacter::StartRangeCombo()
{
	// 如果当前攻击命中敌人, 可继续追击
	if (bIsAttackHit)
	{
		return TryRangeAttack();
	}

	return false;
}

void AMonsterCharacter::SetMonsterInfo(const FMonsterBaseInfoStruct& MonsterBaseInfo, const FMonsterPatrolInfoStruct& MonsterPatrolInfo)
{
	if (BehaviorComponent)
	{
		BehaviorComponent->IdleType = MonsterBaseInfo.IdleType;
		BehaviorComponent->PatrolType = MonsterPatrolInfo.PatrolType;
		BehaviorComponent->PatrolRouteName = MonsterPatrolInfo.PatrolRouteName;
		// 更新巡逻路径
		BehaviorComponent->UpdatePatrolRoute();
	}
	Loots = MonsterBaseInfo.Loots;
}

void AMonsterCharacter::SpawnLoot()
{
	// 掉落物生成位置相对包围盒的拓展范围
	FVector SpawnBoxExtent = FVector(40.0f, 40.0f, 40.0f);
	// 生成参数
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (auto& CurLoot : Loots)
	{
		// 根据掉落率计算当前是否掉落该物品
		bool IsDrop = UKismetMathLibrary::RandomBoolWithWeight(CurLoot.DropRate);

		if (IsDrop)
		{
			// 计算生成数量
			int32 SpawnCount = FMath::RandRange(CurLoot.MinDropCount, CurLoot.MaxDropCount);
			for (int32 i = 0; i < SpawnCount; ++i)
			{
				// 掉落物生成位置
				FVector SpawnPosition = GetActorLocation();
				// 修正生成位置高度
				SpawnPosition.Z -= 50.0f;
				FTransform SpawnTransform(UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), SpawnBoxExtent));
				GetWorld()->SpawnActor<AActor>(CurLoot.LootBP, SpawnTransform, Params);
			}
		}
	}
}