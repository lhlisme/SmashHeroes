// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/BaseAttributeSet.h"
#include "Abilities/SHAbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffectExtension.h"
#include "Characters/BaseCharacter.h"


UBaseAttributeSet::UBaseAttributeSet()
	: Health(1.0f)
	, MaxHealth(1.0f)
	, Energy(0.0f)
	, MaxEnergy(0.0f)
	, MoveSpeed(1.0f)
	, Absorption(0.0f)
	, AttackMultiplier(1.0f)
	, DefenseMultiplier(0.0f)
	, BaseAttackPower(1.0f)
	, BaseDefensePower(1.0f)
	, DefenseRange(220.0f)
	, Damage(0.0f)
{
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseAttributeSet, Health);
	DOREPLIFETIME(UBaseAttributeSet, MaxHealth);
	DOREPLIFETIME(UBaseAttributeSet, Energy);
	DOREPLIFETIME(UBaseAttributeSet, MaxEnergy);
	DOREPLIFETIME(UBaseAttributeSet, MoveSpeed);
	DOREPLIFETIME(UBaseAttributeSet, Absorption);
	DOREPLIFETIME(UBaseAttributeSet, AttackMultiplier);
	DOREPLIFETIME(UBaseAttributeSet, DefenseMultiplier);
	DOREPLIFETIME(UBaseAttributeSet, BaseAttackPower);
	DOREPLIFETIME(UBaseAttributeSet, BaseDefensePower);
	DOREPLIFETIME(UBaseAttributeSet, DefenseRange);
}

void UBaseAttributeSet::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health);
}

void UBaseAttributeSet::OnRep_MaxHealth()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth);
}

void UBaseAttributeSet::OnRep_Energy()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Energy);
}

void UBaseAttributeSet::OnRep_MaxEnergy()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxEnergy);
}

void UBaseAttributeSet::OnRep_MoveSpeed()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MoveSpeed);
}

void UBaseAttributeSet::OnRep_Absorption()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Absorption);
}

void UBaseAttributeSet::OnRep_AttackMultiplier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, AttackMultiplier);
}

void UBaseAttributeSet::OnRep_DefenseMultiplier()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DefenseMultiplier);
}

void UBaseAttributeSet::OnRep_BaseAttackPower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BaseAttackPower);
}

void UBaseAttributeSet::OnRep_BaseDefensePower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BaseDefensePower);
}

void UBaseAttributeSet::OnRep_DefenseRange()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DefenseRange);
}

void UBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute()) {
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxEnergyAttribute()) {
		AdjustAttributeForMaxChange(Energy, MaxEnergy, NewValue, GetEnergyAttribute());
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Compute the delta between old and new, if it is available
	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ABaseCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ABaseCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		ABaseCharacter* SourceCharacter = nullptr;
		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// Use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<ABaseCharacter>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<ABaseCharacter>(SourceActor);
			}

			// Set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		// Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

				// Call for all health changes
				TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes such as from healing or direct modifiers
		// First clamp it
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (TargetCharacter)
		{
			// Call for all health changes
			TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		// Clamp mana
		SetEnergy(FMath::Clamp(GetEnergy(), 0.0f, GetMaxEnergy()));

		if (TargetCharacter)
		{
			// Call for all mana changes
			TargetCharacter->HandleEnergyChanged(DeltaValue, SourceTags);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		if (TargetCharacter)
		{
			// Call for all movespeed changes
			TargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
		}
	}
}

