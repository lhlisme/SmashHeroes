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
	/** ��ȡ������������ */
	FGameplayAttribute HealthAttribute();
	FGameplayAttribute AttackMultiplierAttribute();
	FGameplayAttribute DefenseMultiplierAttribute();
	FGameplayAttribute BaseAttackPowerAttribute();

	//�������޸�ǰ�����������ֵΪ�޵ĵ�ǰ����ֵ/��������
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);
	//�����Ա��޸�֮�󣬴������Ʋ������ĸ�֪����������
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data);
};
