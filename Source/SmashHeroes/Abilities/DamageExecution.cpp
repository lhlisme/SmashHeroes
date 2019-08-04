// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/DamageExecution.h"
#include "AbilitySystemComponent.h"
#include "Characters/BaseCharacter.h"


static const BaseDamageStatics& DamageStatics()
{
	static BaseDamageStatics DmgStatics;
	return DmgStatics;
}

UDamageExecution::UDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().AbsorptionDef);
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

	ABaseCharacter* SourceCharacter = SourceActor ? Cast<ABaseCharacter>(SourceActor) : nullptr;
	ABaseCharacter* TargetCharacter = TargetActor ? Cast<ABaseCharacter>(TargetActor) : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	// 获取受击位置信息
	const FHitResult* HitInfo = Spec.GetEffectContext().GetHitResult();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;	// 用于获取所有被捕获属性的值
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage * AttackMultiplier * BaseAttackPower * DefenseMultiplier / BaseDefensePower
	//	BaseDefensePower greater equal than 1.0
	// --------------------------------------

	float Absorption = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AbsorptionDef, EvaluationParameters, Absorption);

	float BaseDefensePower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDefensePowerDef, EvaluationParameters, BaseDefensePower);
	BaseDefensePower = FMath::Max(BaseDefensePower, 1.0f);	// BaseDefensePower在计算时不能低于1.0

	float BaseAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseAttackPowerDef, EvaluationParameters, BaseAttackPower);

	float DefenseMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseMultiplierDef, EvaluationParameters, DefenseMultiplier);

	float AttackMultiplier = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackMultiplierDef, EvaluationParameters, AttackMultiplier);

	float Damage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	float DamageDone = Damage * AttackMultiplier * BaseAttackPower * (1.0f - Absorption) / BaseDefensePower;
	bool bIsBlocked = false;	// 攻击是否被格挡掉
	float EnergyCost = 0.0f;	// 格挡伤害消耗的能量
	EHitReaction HitReaction = EHitReaction::HitFront;

	if (TargetCharacter && HitInfo)
	{
		// 检测攻击是否被格挡, 以及对应受击反应
		TargetCharacter->CheckHitResult(HitInfo->Location, DamageDone, bIsBlocked, EnergyCost, HitReaction);
	}

	if (bIsBlocked)
	{
		// 防御成功, 最终受到的伤害受到防御系数影响
		DamageDone *= (1.0f - DefenseMultiplier);
	}

	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}