// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ----------------------------------------------------------------------------------------------------------------
// ��ͷ�ļ�ͳһ�����Ϸ�еĹ����ṹ����Ϣ
// ----------------------------------------------------------------------------------------------------------------

#include "UObject/PrimaryAssetId.h"
#include "SHTypes.generated.h"

class USHItem;

/** UI����ʾ����Ʒ��۽ṹ */
UCLASS(BlueprintType)
struct SMASHHEROES_API FSHItemSlot
{
	GENERATED_BODY()

	/** Constructor, -1��ʾ��Ч��� */
	FSHItemSlot()
		: SlotNumber(-1)
	{}

	FSHItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
		: ItemType(InItemType)
		, SlotNumber(InSlotNumber)
	{}

	/** ��۶�Ӧ����Ʒ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	/** ��۱��, ��0��ʼ */
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

	/** �������͹�ϣֵ */
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

/** ��Ҳֿ���USHItem�Ķ�����Ϣ */
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

	/** �ֿ���Item��ʵ������, ����С��1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemCount;

	/** Item�ȼ�, ����ʵ������, ����С��1 */
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

	/** ���Item����, Count����, Level���� */
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

/** �ֿ���Item�����仯ʱ���� */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged, bool, bAdded, USHItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChangedNative, bool, USHItem*);

/** ��Ʒ��۵����ݷ����仯ʱ���� */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FSHItemSlot, ItemSlot, USHItem*, Item);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChangedNative, FSHItemSlot, USHItem*);

/** �����ֿ������Ϻ����, ����֮ǰ��Item��Ϣ�ᱻ���� */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);
DECLARE_MULTICAST_DELEGATE(FOnInventoryLoadedNative);

