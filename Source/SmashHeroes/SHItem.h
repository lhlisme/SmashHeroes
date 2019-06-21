// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "SHItem.generated.h"

class USHGameplayAbility;

/** 所有道具类的基类 */
UCLASS(Abstract, BlueprintType)
class SMASHHEROES_API USHItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USHItem()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
		, AbilityLevel(1)
	{}

	/** 道具类型, 在具体道具子类中设置 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	/** 道具名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	/** 道具描述 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	/** 道具显示图标 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FSlateBrush ItemIcon;

	/** 道具价格 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Price;

	/** 单个实例最多拥有该道具的数量, <= 0 表示无上限 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxCount;

	/** 判断道具是否为消耗品(MaxCount <= 0) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Max")
	bool IsConsumable() const;

	/** 道具的最高等级, <= 0 表示无上限 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxLevel;

	/** 道具附加能力列表 */
	typedef TPair<TSubclassOf<USHGameplayAbility>, int32> TGrantedAbilityPair;	// Key: 附加能力, Value: 附加能力对应等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TGrantedAbilityPair> GrantedAbilities;

	/** 获取道具的唯一名称(Primary Asset ID) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
