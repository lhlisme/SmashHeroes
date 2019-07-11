// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SHInventoryInterface.h"
#include "SHPlayerControllerBase.generated.h"

/**
 * Base class for PlayerController, should be blueprinted
 */
UCLASS()
class SMASHHEROES_API ASHPlayerControllerBase : public APlayerController, public ISHInventoryInterface
{
	GENERATED_BODY()
	
private:
	float ForwardInput = 0.0f;
	float RightInput = 0.0f;

public:
	ASHPlayerControllerBase() {}

	virtual void BeginPlay() override;

	/** ��ҵ�ǰӵ����Ʒ��ӳ�� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<USHItem*, FSHItemData> InventoryData;

	/** Slot��Item��ӳ��, ��SHGameInstanceBase��SlotsPerType�����г�ʼ�� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSHItemSlot, USHItem*> SlottedItems;

	/** ������Ʒ�����ı�ʱ�ص� */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** ����ί�е�Native�汾, ��BPί��ǰ���� */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** �����Ʒ�ı�ʱ�ص� */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** ����ί�е�Native�汾, ��BPί��ǰ���� */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	/** ����������Ϻ�ص� */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** ����ί�е�Native�汾, ��BPί��ǰ���� */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	/** ������Ʒ�ı� */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, USHItem* Item);

	/** Slot��Ʒ�ı� */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void SlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item);

	/** ���һ���µ���Ʒ�������� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(USHItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** �ӱ������Ƴ�һ����Ʒ, ͬʱ�Ὣ���Slot���Ƴ���RemoveCount <= 0ʱ��ʾ�Ƴ����и��� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItem(USHItem* RemovedItem, int32 RemoveCount = 1);

	/** ��ȡ������ָ�����͵�����Item, ��δָ������, �򷵻�����Item */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetInventoryItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType);

	/** ��ȡָ��Item�ڱ����е����� */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetInventoryItemCount(USHItem* Item) const;

	/** ��ȡItem��Ӧ��ItemData, ��δ�ҵ�, ����false */
	UFUNCTION(BlueprintPure, Category = Inventory)
	bool GetInventoryItemData(USHItem* Item, FSHItemData& ItemData) const;

	/** ����Slot��Ӧ��Item, ���ItemΪ��, �����Slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SetSlottedItem(FSHItemSlot ItemSlot, USHItem* Item);

	/** ��ȡSlot��Ӧ��Item */
	UFUNCTION(BlueprintPure, Category = Inventory)
	USHItem* GetSlottedItem(FSHItemSlot ItemSlot) const;

	/** ��ȡSlots��ָ�����͵�����Item, ��δָ������, �򷵻�����Item */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetSlottedItems(TArray<USHItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** ʹ�ñ����е�Item���յ�Slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void FillEmptySlots();

	/** ���汳����Ϣ */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** ���ݴ浵���ر�����Ϣ */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

	// ʵ�ֽӿ�ISHInventoryInterface
	virtual const TMap<USHItem*, FSHItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	virtual const TMap<FSHItemSlot, USHItem*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
	{
		return OnInventoryItemChangedNative;
	}
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}

protected:
	/** ��ָ��Item�Զ�������ʵ�Slot, ��������ݸı�, ����true */
	bool FillEmptySlotWithItem(USHItem* NewItem);

	/** ���ñ������»ص� */
	void NotifyInventoryItemChanged(bool bAdded, USHItem* Item);
	void NotifySlottedItemChanged(FSHItemSlot ItemSlot, USHItem* Item);
	void NotifyInventoryLoaded();
};
