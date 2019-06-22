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
	// ��Ϊ������ͼ���迼��Name��"_C"��׺
	return FPrimaryAssetId(ItemType, GetFName());
}
