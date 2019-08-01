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
	/** 当前生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	/** 最大生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

	/** 当前能量值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Energy)

	/** 最大能量值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxEnergy)

	/** 移动速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)

	/** 减伤率(1.0表示免伤) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_Absorption)
	FGameplayAttributeData Absorption;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Absorption)

	/** 攻击系数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_AttackMultiplier)
	FGameplayAttributeData AttackMultiplier;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AttackMultiplier)

	/** 防御系数(1.0表示免伤, 角色防御时生效) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_DefenseMultiplier)
	FGameplayAttributeData DefenseMultiplier;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DefenseMultiplier)

	/** 基础攻击力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_BaseAttackPower)
	FGameplayAttributeData BaseAttackPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BaseAttackPower)

	/** 基础防御力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Attributes", ReplicatedUsing = OnRep_BaseDefensePower)
	FGameplayAttributeData BaseDefensePower;
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
	virtual void OnRep_Absorption();

	UFUNCTION()
	virtual void OnRep_AttackMultiplier();

	UFUNCTION()
	virtual void OnRep_DefenseMultiplier();

	UFUNCTION()
	virtual void OnRep_BaseAttackPower();

	UFUNCTION()
	virtual void OnRep_BaseDefensePower();
};
