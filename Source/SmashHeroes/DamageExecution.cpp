// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecution.h"
#include "AbilitySystemComponent.h"


UDamageExecution::UDamageExecution(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttrStruct Attributes;

	RelevantAttributesToCapture.Add(Attributes.HealthDef); //RelevantAttributesToCapture is the array that contains all attributes you wish to capture, without exceptions. 
	InvalidScopedModifierAttributes.Add(Attributes.HealthDef); //However, an attribute added here on top of being added in RelevantAttributesToCapture will still be captured, but will not be shown for potential in-function modifiers in the GameplayEffect blueprint, more on that later.

	RelevantAttributesToCapture.Add(Attributes.BaseAttackPowerDef);
	RelevantAttributesToCapture.Add(Attributes.DefenseMultiplierDef);
	RelevantAttributesToCapture.Add(Attributes.AttackMultiplierDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	AttrStruct Attributes;	// Creating the attribute struct, we will need its values later when we want to get the attribute values.

	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();	
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();	// ��ȡSource������Tags
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;	// ���ڻ�ȡ���б��������Ե�ֵ
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.HealthDef, EvaluationParameters, Health);

	float BaseAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.BaseAttackPowerDef, EvaluationParameters, BaseAttackPower);

	float AttackMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.AttackMultiplierDef, EvaluationParameters, AttackMultiplier);

	float DefenseMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attributes.DefenseMultiplierDef, EvaluationParameters, DefenseMultiplier);

	float DamageDone = BaseAttackPower * AttackMultiplier * DefenseMultiplier;
	DamageDone = FMath::Min<float>(DamageDone, Health);	// ��ֹ����ֵ����0��Ҳ����BaseAttributeSet��PostGameplayEffectExecution�д���

	if (DamageDone > 0.f)
	{
		//TODO Damage()��λ�ȡ���룿
		//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Damage().HealthAttribute(), EGameplayModOp::Additive, -DamageDone));
	}
}