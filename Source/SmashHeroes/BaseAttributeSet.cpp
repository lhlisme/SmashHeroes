// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"


FGameplayAttribute UBaseAttributeSet::HealthAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, Health));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::AttackMultiplierAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, AttackMultiplier));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::DefenseMultiplierAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, DefenseMultiplier));
	return FGameplayAttribute(Property);
}

FGameplayAttribute UBaseAttributeSet::BaseAttackPowerAttribute()
{
	static UProperty* Property = FindFieldChecked<UProperty>(UBaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UBaseAttributeSet, BaseAttackPower));
	return FGameplayAttribute(Property);
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// 如果待修改的属性为Health属性，则进行Clamp
	if (Attribute == HealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, 100.0f);

	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	UAbilitySystemComponent* Source = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	if (Data.EvaluatedData.Attribute  == HealthAttribute()) 
	{
		// Get the Target actor
		AActor* DamagedActor = nullptr;
		AController* DamagedController = nullptr;

		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			DamagedActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			DamagedController = Data.Target.AbilityActorInfo->PlayerController.Get();
		}

		// Get the Source actor
		AActor* AttackingActor = nullptr;
		AController* AttackingController = nullptr;

		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid()) {
			AttackingActor = Source->AbilityActorInfo->AvatarActor.Get();
			AttackingController = Source->AbilityActorInfo->PlayerController.Get();
			if (AttackingController == nullptr && AttackingActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(AttackingActor))
				{
					AttackingController = Pawn->GetController();
				}
			}
		}

		// Clamp health
		//Health = FMath::Clamp(Health, 0.0f, MaxHealth);
		//if (Health <= 0)
		//{
		//	// Handle death with GASCharacter. Note this is just one example of how this could be done.
		//	if (AGASCharacter* GASChar = Cast<AGASCharacter>(DamagedActor))
		//	{
		//		// Construct a gameplay cue event for this death
		//		FGameplayCueParameters Params(Data.EffectSpec.GetContext());
		//		Params.RawMagnitude = Data.EvaluatedData.Magnitude;;
		//		Params.NormalizedMagnitude = FMath::Abs(Data.EvaluatedData.Magnitude / MaxHealth);
		//		Params.AggregatedSourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
		//		Params.AggregatedTargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();

		//		GASChar->Die(DamagedController, DamagedActor, Data.EffectSpec, Params.RawMagnitude, Params.Normal);
		//	}
		//}
	}
}