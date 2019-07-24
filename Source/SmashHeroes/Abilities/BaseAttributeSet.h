// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * This holds all of the attributes used by abilities, it instantiates a copy of this on every character
 */
UCLASS()
class SMASHHEROES_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	// Hitpoints. Self-explanatory.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;	// 当前生命值
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;	// 最大生命值
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;	// 当前能量值
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Energy)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;	// 最大能量值
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxEnergy)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;	// 移动速度
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)

	// Outgoing damage-multiplier.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_AttackMultiplier)
	FGameplayAttributeData AttackMultiplier;	// 攻击系数
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackMultiplier)

	// Incoming damage-multiplier.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_DefenseMultiplier)
	FGameplayAttributeData DefenseMultiplier;	// 防御系数(1.0表示免伤, 角色防御时生效)
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DefenseMultiplier)

	// Base damage of an outgoing attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_BaseAttackPower)
	FGameplayAttributeData BaseAttackPower;		// 基础攻击力
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BaseAttackPower)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_BaseDefensePower)
	FGameplayAttributeData BaseDefensePower;	// 基础防御力
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BaseDefensePower)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Base Attributes", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)

public:
	UBaseAttributeSet();

	//在属性修改前，处理以最大值为限的当前生命值/法力调整
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//在属性被修改之后，处理限制并将更改告知给其他对象
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	// These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	UFUNCTION()
	virtual void OnRep_Health();

	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_Energy();

	UFUNCTION()
	virtual void OnRep_MaxEnergy();

	UFUNCTION()
	virtual void OnRep_MoveSpeed();

	UFUNCTION()
	virtual void OnRep_AttackMultiplier();

	UFUNCTION()
	virtual void OnRep_DefenseMultiplier();

	UFUNCTION()
	virtual void OnRep_BaseAttackPower();

	UFUNCTION()
	virtual void OnRep_BaseDefensePower();
};
