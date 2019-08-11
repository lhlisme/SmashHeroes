// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/AssetManager.h"
#include "SHAssetManager.generated.h"

class USHItem;

/**
 * 
 */
UCLASS()
class SMASHHEROES_API USHAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	USHAssetManager() {}
	virtual void StartInitialLoading() override;

	/** Static types for items */
	static const FPrimaryAssetType	PotionItemType;
	static const FPrimaryAssetType	SkillItemType;
	static const FPrimaryAssetType	TokenItemType;
	static const FPrimaryAssetType	WeaponItemType;

	/** 获取此单例对象 */
	static USHAssetManager& Get();

	/**
	 * 同步加载USHItem子类(不会增加对Item的引用计数, 不影响GC)
	 * @param PrimaryAssetId 待加载的资源ID
	 * @param bDisplayWarning 加载失败时是否输出警告
	 */
	USHItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

};
