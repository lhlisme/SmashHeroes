// Fill out your copyright notice in the Description page of Project Settings.


#include "SHAssetManager.h"
#include "Items/SHItem.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	USHAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	USHAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	USHAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	USHAssetManager::WeaponItemType = TEXT("Weapon");

USHAssetManager& USHAssetManager::Get()
{
	USHAssetManager* This = Cast<USHAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<USHAssetManager>(); // never calls this
	}
}

void USHAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}


USHItem* USHAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// 同步加载, 可能挂起
	USHItem* LoadedItem = Cast<USHItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}