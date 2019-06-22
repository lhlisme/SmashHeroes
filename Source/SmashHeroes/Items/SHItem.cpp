// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SHItem.h"

bool USHItem::IsConsumable() const
{
	if (MaxCount <= 0)
	{
		return true;
	}
	return false;
}

FString USHItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId USHItem::GetPrimaryAssetId() const
{
	// 因为不是蓝图无需考虑Name的"_C"后缀
	return FPrimaryAssetId(ItemType, GetFName());
}
