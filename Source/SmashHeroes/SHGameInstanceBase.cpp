// Fill out your copyright notice in the Description page of Project Settings.

#include "SHGameInstanceBase.h"
#include "SHAssetManager.h"
#include "SHSaveGame.h"
#include "Items/SHItem.h"
#include "Kismet/GameplayStatics.h"

USHGameInstanceBase::USHGameInstanceBase()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{}

void USHGameInstanceBase::AddDefaultInventory(USHSaveGame* SaveGame, bool bRemoveExtra)
{
	// If we want to remove extra, clear out the existing inventory
	if (bRemoveExtra)
	{
		SaveGame->InventoryData.Reset();
	}

	// Now add the default inventory, this only adds if not already in hte inventory
	for (const TPair<FPrimaryAssetId, FSHItemData>& Pair : DefaultInventory)
	{
		if (!SaveGame->InventoryData.Contains(Pair.Key))
		{
			SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
		}
	}
}

bool USHGameInstanceBase::IsValidItemSlot(FSHItemSlot ItemSlot) const
{
	if (ItemSlot.IsValid())
	{
		const int32* FoundCount = ItemSlotsPerType.Find(ItemSlot.ItemType);

		if (FoundCount)
		{
			return ItemSlot.SlotNumber < *FoundCount;
		}
	}
	return false;
}

USHSaveGame* USHGameInstanceBase::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void USHGameInstanceBase::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool USHGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<USHSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(CurrentSaveGame, false);

		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<USHSaveGame>(UGameplayStatics::CreateSaveGameObject(USHSaveGame::StaticClass()));

		AddDefaultInventory(CurrentSaveGame, true);

		return false;
	}
}

bool USHGameInstanceBase::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex);
	}
	return false;
}

void USHGameInstanceBase::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}