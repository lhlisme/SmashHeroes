// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "SHInventoryInterface.generated.h"

/**
* SHItems�󶨵�ItemSlots��ϵ�нӿ�
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
	/** ����Item��Data��ӳ�� */
	virtual const TMap<USHItem*, FSHItemData>& GetInventoryDataMap() const = 0;

	/** ����Slots��Items��ӳ�� */
	virtual const TMap<FSHItemSlot, USHItem*>& GetSlottedItemMap() const = 0;

	/** ��ȡItem�ı�ʱ���õ�ί�� */
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() = 0;

	/** ��ȡSlot�ı�ʱ���õ�ί�� */
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() = 0;

	/** ��ȡ�������ݼ�����ϵ��õ�ί�� */
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() = 0;

};
