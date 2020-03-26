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
UENUM(BlueprintType)
enum class EHitReaction : uint8
{
	/** 格挡攻击 */
	BlockHit			UMETA(DisplayName = "BlockHit"),
	/** 击中左方 */
	HitLeft				UMETA(DisplayName = "HitLeft"),
	/** 击中右方 */
	HitRight			UMETA(DisplayName = "HitRight"),
	/** 防御击破 */
	GuardBreak			UMETA(DisplayName = "GuardBreak"),
	/** 击退 */
	KnockBack			UMETA(DisplayName = "KnockBack"),
	/** 击倒 */
	KnockDown			UMETA(DisplayName = "KnockDown"),
	/** 击飞 */
	KnockUp				UMETA(DisplayName = "KnockUp"),
	/** 无反馈 */
	NoReaction          UMETA(DisplayName = "NoReaction")
};

/** 移动方向 */
UENUM(BlueprintType)
enum class EMoveDirection : uint8
{
	Forward				UMETA(DisplayName = "Forward"),
	Back				UMETA(DisplayName = "Back"),
	Left				UMETA(DisplayName = "Left"),
	Right				UMETA(DisplayName = "Right"),
	LeftForward			UMETA(DisplayName = "LeftForward"),
	RightForward		UMETA(DisplayName = "RightForward"),
	LeftBack			UMETA(DisplayName = "LeftBack"),
	RightBack			UMETA(DisplayName = "RightBack")
};

