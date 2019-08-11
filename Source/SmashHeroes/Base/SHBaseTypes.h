// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHBaseTypes.generated.h"

/**
 * ���ͨ�ýṹ����
 */

/** ����ǿ�� */
UENUM(BlueprintType)
enum class EAttackStrength : uint8
{
	LightAttack					UMETA(DisplayName = "LightAttack"),
	NormalAttack				UMETA(DisplayName = "NormalAttack"),
	HeavyAttack					UMETA(DisplayName = "HeavyAttack")
};
