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

	/** ��ȡ�˵������� */
	static USHAssetManager& Get();

	/**
	 * ͬ������USHItem����(�������Ӷ�Item�����ü���, ��Ӱ��GC)
	 * @param PrimaryAssetId �����ص���ԴID
	 * @param bDisplayWarning ����ʧ��ʱ�Ƿ��������
	 */
	USHItem* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);

};
