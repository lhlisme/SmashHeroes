// Fill out your copyright notice in the Description page of Project Settings.

#include "SHSaveGame.h"
#include "SHGameInstanceBase.h"

void USHSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != ESHSaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < ESHSaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
			{
				InventoryData.Add(ItemId, FSHItemData(1, 1));
			}

			InventoryItems_DEPRECATED.Empty();
		}

		SavedDataVersion = ESHSaveGameVersion::LatestVersion;
	}
}