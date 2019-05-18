// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "BaseAttributeSet.h"
#include "DamageExecution.generated.h"

struct AttrStruct
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health); //The DECLARE_ATTRIBUTE_CAPTUREDEF macro actually only declares two variables. The variable names are dependent on the input, however. Here they will be HealthProperty(which is a UPROPERTY pointer)
																		 //and HealthDef(which is a FGameplayEffectAttributeCaptureDefinition).
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackMultiplier); //Here AttackMultiplierProperty and AttackMultiplierDef. I hope you get the drill.
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefenseMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BaseAttackPower);

	AttrStruct()
	{
		// We define the values of the variables we declared now. In this example, HealthProperty will point to the Health attribute in the UMyAttributeSet on the receiving target of this execution. The last parameter is a bool, and determines if we snapshot the attribute's value at the time of definition.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);

		//This here is a different example: We still take the attribute from UMyAttributeSet, but this time it is BaseAttackPower, and we look at the effect's source for it. We also want to snapshot is because the effect's strength should be determined during its initial creation. A projectile wouldn't change
				//damage values depending on the source's stat changes halfway through flight, after all.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BaseAttackPower, Source, true);

		//The same rules apply for the multiplier attributes too.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, AttackMultiplier, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, DefenseMultiplier, Target, false);
	}
};

/**
 * 
 */
UCLASS()
class SMASHHEROES_API UDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
