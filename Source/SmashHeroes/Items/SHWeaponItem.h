// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Items/SHItem.h"
#include "SHWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHWeaponItem : public USHItem
{
	GENERATED_BODY()
	
public:
	USHWeaponItem()
	{
		ItemType = USHAssetManager::WeaponItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> WeaponActor;
};
