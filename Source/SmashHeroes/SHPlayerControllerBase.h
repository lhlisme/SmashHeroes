// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SHInventoryInterface.h"
#include "SHPlayerControllerBase.generated.h"

/**
 * Base class for PlayerController, should be blueprinted
 */
UCLASS()
class SMASHHEROES_API ASHPlayerControllerBase : public APlayerController, public ISHInventoryInterface
{
	GENERATED_BODY()
	
private:
	float ForwardInput = 0.0f;
	float RightInput = 0.0f;

public:
	ASHPlayerControllerBase() {}

	virtual void BeginPlay() override;

	/** 玩家当前拥有物品的映射 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<USHItem*, FSHItemData> InventoryData;

	/** Slot到Item的映射, 在SHGameInstanceBase的SlotsPerType方法中初始化 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSHItemSlot, USHItem*> SlottedItems;

	/** 背包物品发生改变时回调 */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** 上述委托的Native版本, 在BP委托前调用 */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** 插槽物品改变时回调 */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** 上述委托的Native版本, 在BP委托前调用 */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	/** 背包加载完毕后回调 */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** 上述委托的Native版本, 在BP委托前调用 */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	/** 背包物品改变 */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, USHItem* Item);

	/** Slot物品改变 */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void SlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item);

	/** 添加一项新的物品到背包中 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(USHItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** 从背包中移除一项物品, 同时会将其从Slot中移除。RemoveCount <= 0时表示移除所有副本 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItem(USHItem* RemovedItem, int32 RemoveCount = 1);

	/** 获取背包中指定类型的所有Item, 若未指定类型, 则返回所有Item */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetInventoryItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType);

	/** 获取指定Item在背包中的数量 */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetInventoryItemCount(USHItem* Item) const;

	/** 获取Item对应的ItemData, 若未找到, 返回false */
	UFUNCTION(BlueprintPure, Category = Inventory)
	bool GetInventoryItemData(USHItem* Item, FSHItemData& ItemData) const;

	/** 设置Slot对应的Item, 如果Item为空, 则清空Slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SetSlottedItem(FSHItemSlot ItemSlot, USHItem* Item);

	/** 获取Slot对应的Item */
	UFUNCTION(BlueprintPure, Category = Inventory)
	USHItem* GetSlottedItem(FSHItemSlot ItemSlot) const;

	/** 获取Slots中指定类型的所有Item, 若未指定类型, 则返回所有Item */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetSlottedItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** 使用背包中的Item填充空的Slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void FillEmptySlots();

	/** 保存背包信息 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** 根据存档加载背包信息 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

	// 实现接口ISHInventoryInterface
	virtual const TMap<USHItem*, FSHItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	virtual const TMap<FSHItemSlot, USHItem*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
	{
		return OnInventoryItemChangedNative;
	}
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}

protected:
	/** 将指定Item自动填入合适的Slot, 如果有数据改变, 返回true */
	bool FillEmptySlotWithItem(USHItem* NewItem);

	/** 调用背包更新回调 */
	void NotifyInventoryItemChanged(bool bAdded, USHItem* Item);
	void NotifySlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item);
	void NotifyInventoryLoaded();
};
