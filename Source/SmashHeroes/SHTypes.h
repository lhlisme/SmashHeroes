// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ----------------------------------------------------------------------------------------------------------------
// 此头文件统一存放游戏中的公共结构体信息
// ----------------------------------------------------------------------------------------------------------------

#include "UObject/PrimaryAssetId.h"
#include "SHTypes.generated.h"

class USHItem;

/** UI中显示的物品插槽结构 */
UCLASS(BlueprintType)
struct SMASHHEROES_API FSHItemSlot
{
	GENERATED_BODY()

	/** Constructor, -1表示无效插槽 */
	FSHItemSlot()
		: SlotNumber(-1)
	{}

	FSHItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
		: ItemType(InItemType)
		, SlotNumber(InSlotNumber)
	{}

	/** 插槽对应的物品类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	/** 插槽编号, 从0开始 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	bool operator==(const FSHItemSlot& Other) const
	{
		return ItemType == Other.ItemType && SlotNumber == Other.SlotNumber;
	}

	bool operator!=(const FSHItemSlot& Other) const
	{
		return !(*this == Other);
	}

	/** 计算类型哈希值 */
	friend inline uint32 GetTypeHash(const FSHItemSlot& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, (uint32)Key.SlotNumber);

		return Hash;
	}

	bool IsValid() const
	{
		return ItemType.IsValid() && SlotNumber >= 0;
	}
};

/** 玩家仓库中USHItem的额外信息 */
USTRUCT(BlueprintType)
struct SMASHHEROES_API FSHItemData
{
	GENERATED_BODY()

	FSHItemData()
		: ItemCount(1)
		, ItemLevel(1)
	{}

	FSHItemData(int32 InItemCount, int32 InItemLevel)
		: ItemCount(InItemCount)
		, ItemLevel(InItemLevel)
	{}

	/** 仓库中Item的实例数量, 不能小于1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemCount;

	/** Item等级, 所有实例共享, 不能小于1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemLevel;

	bool operator==(const FSHItemData& Other) const
	{
		return ItemCount == Other.ItemCount && ItemLevel == Other.ItemLevel;
	}

	bool operator!=(const FSHItemData& Other) const
	{
		return !(*this == Other);
	}

	bool IsValid() const
	{
		return ItemCount > 0;
	}

	/** 添加Item数据, Count增加, Level覆盖 */
	void UpdateItemData(const FSHItemData& Other, int32 MaxCount, int32 MaxLevel)
	{
		if (MaxCount <= 0)
		{
			MaxCount = MAX_int32;
		}

		if (MaxLevel <= 0)
		{
			MaxLevel = MAX_int32;
		}

		ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, MaxCount);
		ItemLevel = FMath::Clamp(Other.ItemLevel, 1, MaxLevel);
	}
};

/** 仓库中Item发生变化时调用 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, bool, bAdded, USHItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedNative, bool, USHItem*);

/** 物品插槽的内容发生变化时调用 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FSHItemSlot, ItemSlot, USHItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedNative, FSHItemSlot, USHItem*);

/** 整个仓库加载完毕后调用, 所有之前的Item信息会被覆盖 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoadedNative);

