// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Items/SHItem.h"
#include "SHPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHPotionItem : public USHItem
{
	GENERATED_BODY()

public:
	USHPotionItem()
	{
		ItemType = USHAssetManager::PotionItemType;
	}
};
