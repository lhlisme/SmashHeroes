// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	// Hitpoints. Self-explanatory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData Health;

	// Outgoing damage-multiplier.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", meta = (HideFromModifiers))
	FGameplayAttributeData AttackMultiplier;

	// Incoming damage-multiplier.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData DefenseMultiplier;

	// Base damage of an outgoing attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes")
	FGameplayAttributeData BaseAttackPower;

public:
	/** 获取各项属性类型 */
	FGameplayAttribute HealthAttribute();
	FGameplayAttribute AttackMultiplierAttribute();
	FGameplayAttribute DefenseMultiplierAttribute();
	FGameplayAttribute BaseAttackPowerAttribute();

	//在属性修改前，处理以最大值为限的当前生命值/法力调整
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	//在属性被修改之后，处理限制并将更改告知给其他对象
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data);
};
