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

/** 受击反馈 */
UENUM()
enum class EHitReaction : uint8
{
	BlockHit			UMETA(DisplayName = "BlockHit"),		// 格挡攻击
	HitLeft				UMETA(DisplayName = "HitLeft"),			// 击中左方
	HitRight			UMETA(DisplayName = "HitRight"),		// 击中右方
	GuardBreak			UMETA(DisplayName = "GuardBreak"),		// 防御击破
	KnockBack			UMETA(DisplayName = "KnockBack"),		// 击退
	KnockDown			UMETA(DisplayName = "KnockDown"),		// 击倒
	KnockUp				UMETA(DisplayName = "KnockUp"),			// 击飞
	NoReaction          UMETA(DisplayName = "NoReaction")		// 无反馈
};

