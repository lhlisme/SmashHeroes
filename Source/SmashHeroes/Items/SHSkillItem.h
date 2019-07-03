// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Items/SHItem.h"
#include "SHSkillItem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHSkillItem : public USHItem
{
	GENERATED_BODY()
	
public:
	USHSkillItem()
	{
		ItemType = USHAssetManager::SkillItemType;
	}
};
