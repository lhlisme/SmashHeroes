// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHBaseTypes.generated.h"

/**
 * 存放通用结构定义
 */

/** 攻击强度 */
UENUM(BlueprintType)
enum class EAttackStrength : uint8
{
	LightAttack					UMETA(DisplayName = "LightAttack"),
	NormalAttack				UMETA(DisplayName = "NormalAttack"),
	HeavyAttack					UMETA(DisplayName = "HeavyAttack")
};
