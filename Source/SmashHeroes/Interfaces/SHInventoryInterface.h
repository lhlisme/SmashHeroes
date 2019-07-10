// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "SHInventoryInterface.generated.h"

/**
* SHItems绑定到ItemSlots的系列接口
*/
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USHInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class SMASHHEROES_API ISHInventoryInterface
{
	GENERATED_BODY()
	
public:	
	/** 返回Item到Data的映射 */
	virtual const TMap<USHItem*, FSHItemData>& GetInventoryDataMap() const = 0;

	/** 返回Slots到Items的映射 */
	virtual const TMap<FSHItemSlot, USHItem*>& GetSlottedItemMap() const = 0;

	/** 获取Item改变时调用的委托 */
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() = 0;

	/** 获取Slot改变时调用的委托 */
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() = 0;

	/** 获取背包数据加载完毕调用的委托 */
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() = 0;

};
