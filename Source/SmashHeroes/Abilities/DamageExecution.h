// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Abilities/BaseAttributeSet.h"
#include "DamageExecution.generated.h"


struct BaseDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseDefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseAttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	BaseDamageStatics()
	{
		// Capture the Target's BaseDefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DefenseMultiplier, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BaseDefensePower, Target, false);

		// Capture the Source's BaseAttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, AttackMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BaseAttackPower, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Damage, Source, true);
	}
};

/**
 * A damage execution, which allows doing damage by combining a raw Damage number with AttackMultiplier and DefenseMultiplier
 */
UCLASS()
class SMASHHEROES_API UDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UDamageExecution();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
