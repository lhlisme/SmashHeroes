// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Items/SHItem.h"
#include "SHTokenItem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHTokenItem : public USHItem
{
	GENERATED_BODY()
	
public:
	USHTokenItem()
	{
		ItemType = USHAssetManager::TokenItemType;
		MaxCount = 0; // 数量无上限
	}
};
