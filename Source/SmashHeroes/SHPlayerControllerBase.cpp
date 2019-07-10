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

	// ��ȡ��ǰItemData
	FSHItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// ��ȡ�޸ĺ��ItemData
	FSHItemData NewData = OldData;
	NewData.UpdateItemData(FSHItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// ����������޸�, ����²������ص�
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// ��Item�Զ�����Slot
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// ������޸�, ����±�����Ϣ
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

	// ��ȡ��ǰItemData
	FSHItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		return false;
	}

	// ��RemoveCount <= 0, ɾ������Item
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
		// ��������
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// ��ȫɾ��Item, ȷ���䲻��Slot��
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

			// ����Item Type����
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
			// ��Item�����ָ��Slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// ��Item�Ӿɵ�Slot���Ƴ�
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
		// ��д��֮ǰ, ����Save Game�еĻ�����Ϣ
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

		// ���������Ѹ���, ����д��Ӳ��
		GameInstance->WriteSaveGame();
		
		return true;
	}

	return false;
}

bool ASHPlayerControllerBase::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	// ���Game Instance�е�Slots��Ϣ
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
		// ��Save Game������Controller������
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
			// �Զ����Slots, ��Ϊû��Slot������
			FillEmptySlots();
		}

		NotifyInventoryLoaded();

		return true;
	}

	// ����ʧ�ܵ������˱�����Ϣʱ, ��Ҫ֪ͨUI���и���
	NotifyInventoryLoaded();

	return false;
}

bool ASHPlayerControllerBase::FillEmptySlotWithItem(USHItem* NewItem)
{
	// Ѱ��һ���յ�Slot��װ��Item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FSHItemSlot EmptySlot;
	for (TPair<FSHItemSlot, USHItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item�Ѿ���Slot��
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// ��Item������ʵ�Slot
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
	// ����ͼ֮ǰ֪ͨ
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// ������ͼ�����¼�
	InventoryItemChanged(bAdded, Item);
}

void ASHPlayerControllerBase::NotifySlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item)
{
	// ����ͼ֮ǰ֪ͨ
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// ������ͼ�����¼�
	SlottedItemChanged(ItemSlot, Item);
}

void ASHPlayerControllerBase::NotifyInventoryLoaded()
{
	// ����ͼ֮ǰ֪ͨ
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void ASHPlayerControllerBase::BeginPlay()
{
	// �Ӵ浵���ر�����Ʒ
	LoadInventory();

	Super::BeginPlay();
}
