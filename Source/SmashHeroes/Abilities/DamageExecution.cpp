// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DamageExecution.h"
#include "AbilitySystemComponent.h"


static const BaseDamageStatics& DamageStatics()
{
	static BaseDamageStatics DmgStatics;
	return DmgStatics;
}

UDamageExecution::UDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenseMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().BaseDefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatics().BaseAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();	
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;	// ���ڻ�ȡ���б��������Ե�ֵ
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage * AttackMultiplier * BaseAttackPower * DefenseMultiplier / BaseDefensePower
	//	BaseDefensePower greater equal than 1.0
	// --------------------------------------

	float BaseDefensePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDefensePowerDef, EvaluationParameters, BaseDefensePower);
	BaseDefensePower = FMath::Max(BaseDefensePower, 1.0f);	// BaseDefensePower�ڼ���ʱ���ܵ���1.0

	float BaseAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseAttackPowerDef, EvaluationParameters, BaseAttackPower);

	float DefenseMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseMultiplierDef, EvaluationParameters, DefenseMultiplier);

	float AttackMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackMultiplierDef, EvaluationParameters, AttackMultiplier);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	float DamageDone = Damage * AttackMultiplier * BaseAttackPower * DefenseMultiplier / BaseDefensePower;
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}