// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/GameInstance.h"
#include "SHGameInstanceBase.generated.h"

class USHItem;
class USHSaveGame;

/**
 * GameInstance基类, 需要蓝图化
 * 记得在Project Setting中使用当前类对应的蓝图
 */
UCLASS()
class SMASHHEROES_API USHGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USHGameInstanceBase();

	/** 玩家对应的背包物品列表 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetId, FSHItemData> DefaultInventory;

	/** 每种Item类型对应的Slots数量 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetType, int32> ItemSlotsPerType;

	/** 需要保存的Slot的名称 */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	/** 特定于平台的用户索引 */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	/** 添加默认背包信息到背包数组中。若bRemoveExtra为true, 移除默认背包之外的所有背包数据 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultInventory(USHSaveGame* SaveGame, bool bRemoveExtra = false);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsValidItemSlot(FSHItemSlot ItemSlot) const;

	UFUNCTION(BlueprintCallable, Category = Save)
	USHSaveGame* GetCurrentSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void SetSavingEnabled(bool bEnabled);

	/** 加载SaveGame, 若加载失败则新建(加载成功返回true, 新建返回false)*/
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame();

protected:
	/** 当前Save Game对象 */
	UPROPERTY()
	USHSaveGame* CurrentSaveGame;

	/** 是否需要存档 */
	UPROPERTY()
	bool bSavingEnabled;
};
