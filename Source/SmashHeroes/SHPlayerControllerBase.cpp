// Fill out your copyright notice in the Description page of Project Settings.


#include "SHPlayerControllerBase.h"
#include "Characters/BaseCharacter.h"
#include "SHGameInstanceBase.h"
#include "SHSaveGame.h"
#include "Items/SHItem.h"

bool ASHPlayerControllerBase::AddInventoryItem(USHItem* NewItem, int32 ItemCount, int32 ItemLevel, bool bAutoSlot)
{
	bool bChanged = false;
	if (!NewItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}

	if (ItemCount <= 0 || ItemLevel <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level!"), *NewItem->GetName());
		return false;
	}

	// 获取当前ItemData
	FSHItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// 获取修改后的ItemData
	FSHItemData NewData = OldData;
	NewData.UpdateItemData(FSHItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// 如果数据有修改, 则更新并触发回调
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// 将Item自动加入Slot
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// 如果有修改, 则更新背包信息
		SaveInventory();
		return true;
	}
	return false;
}

bool ASHPlayerControllerBase::RemoveInventoryItem(USHItem* RemovedItem, int32 RemoveCount)
{
	if (!RemovedItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null item!"));
		return false;
	}

	// 获取当前ItemData
	FSHItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		return false;
	}

	// 若RemoveCount <= 0, 删除所有Item
	if (RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		// 更新数量
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// 完全删除Item, 确保其不在Slot中
		InventoryData.Remove(RemovedItem);

		for (TPair<FSHItemSlot, USHItem*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	NotifyInventoryItemChanged(false, RemovedItem);

	SaveInventory();
	return true;
}

void ASHPlayerControllerBase::GetInventoryItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<USHItem*, FSHItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			// 根据Item Type过滤
			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

bool ASHPlayerControllerBase::SetSlottedItem(FSHItemSlot ItemSlot, USHItem* Item)
{
	bool bFound = false;
	for (TPair<FSHItemSlot, USHItem*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// 将Item添加至指定Slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// 将Item从旧的Slot中移除
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

int32 ASHPlayerControllerBase::GetInventoryItemCount(USHItem* Item) const
{
	const FSHItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		return FoundItem->ItemCount;
	}

	return 0;
}

bool ASHPlayerControllerBase::GetInventoryItemData(USHItem* Item, FSHItemData& ItemData) const
{
	const FSHItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FSHItemData(0, 0);

	return false;
}

USHItem* ASHPlayerControllerBase::GetSlottedItem(FSHItemSlot ItemSlot) const
{
	USHItem* const* FoundItem = SlottedItems.Find(ItemSlot);

	if (FoundItem)
	{
		return *FoundItem;
	}

	return nullptr;
}

void ASHPlayerControllerBase::GetSlottedItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FSHItemSlot, USHItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void ASHPlayerControllerBase::FillEmptySlots()
{
	bool bShouldSave = false;
	for (const TPair<USHItem*, FSHItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool ASHPlayerControllerBase::SaveInventory()
{
	UWorld* World = GetWorld();
	USHGameInstanceBase* GameInstance = World ? World->GetGameInstance<USHGameInstanceBase>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	USHSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// 在写入之前, 重置Save Game中的缓存信息
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<USHItem*, FSHItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FSHItemSlot, USHItem*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		// 缓存数据已更新, 将其写入硬盘
		GameInstance->WriteSaveGame();
		
		return true;
	}

	return false;
}

bool ASHPlayerControllerBase::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	// 填充Game Instance中的Slots信息
	UWorld* World = GetWorld();
	USHGameInstanceBase* GameInstance = World ? World->GetGameInstance<USHGameInstanceBase>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
		{
			SlottedItems.Add(FSHItemSlot(Pair.Key, SlotNumber), nullptr);
		}
	}

	USHSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	USHAssetManager& AssetManager = USHAssetManager::Get();
	if (CurrentSaveGame)
	{
		// 从Save Game拷贝至Controller数据中
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FSHItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			USHItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FSHItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				USHItem* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			// 自动填充Slots, 因为没有Slot被保存
			FillEmptySlots();
		}

		NotifyInventoryLoaded();

		return true;
	}

	// 加载失败但重置了背包信息时, 需要通知UI进行更新
	NotifyInventoryLoaded();

	return false;
}

bool ASHPlayerControllerBase::FillEmptySlotWithItem(USHItem* NewItem)
{
	// 寻找一个空的Slot来装载Item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FSHItemSlot EmptySlot;
	for (TPair<FSHItemSlot, USHItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item已经在Slot中
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// 将Item插入合适的Slot
				EmptySlot = Pair.Key;
			}
		}
	}

	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}

	return false;
}

void ASHPlayerControllerBase::NotifyInventoryItemChanged(bool bAdded, USHItem* Item)
{
	// 在蓝图之前通知
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// 调用蓝图更新事件
	InventoryItemChanged(bAdded, Item);
}

void ASHPlayerControllerBase::NotifySlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item)
{
	// 在蓝图之前通知
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// 调用蓝图更新事件
	SlottedItemChanged(ItemSlot, Item);
}

void ASHPlayerControllerBase::NotifyInventoryLoaded()
{
	// 在蓝图之前通知
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void ASHPlayerControllerBase::BeginPlay()
{
	// 从存档加载背包物品
	LoadInventory();

	Super::BeginPlay();
}
