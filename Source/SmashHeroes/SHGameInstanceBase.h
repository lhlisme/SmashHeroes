// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/GameInstance.h"
#include "SHGameInstanceBase.generated.h"

class USHItem;
class USHSaveGame;

/**
 * GameInstance����, ��Ҫ��ͼ��
 * �ǵ���Project Setting��ʹ�õ�ǰ���Ӧ����ͼ
 */
UCLASS()
class SMASHHEROES_API USHGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USHGameInstanceBase();

	/** ��Ҷ�Ӧ�ı�����Ʒ�б� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetId, FSHItemData> DefaultInventory;

	/** ÿ��Item���Ͷ�Ӧ��Slots���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FPrimaryAssetType, int32> ItemSlotsPerType;

	/** ��Ҫ�����Slot������ */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	FString SaveSlot;

	/** �ض���ƽ̨���û����� */
	UPROPERTY(BlueprintReadWrite, Category = Save)
	int32 SaveUserIndex;

	/** ���Ĭ�ϱ�����Ϣ�����������С���bRemoveExtraΪtrue, �Ƴ�Ĭ�ϱ���֮������б������� */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddDefaultInventory(USHSaveGame* SaveGame, bool bRemoveExtra = false);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsValidItemSlot(FSHItemSlot ItemSlot) const;

	UFUNCTION(BlueprintCallable, Category = Save)
	USHSaveGame* GetCurrentSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void SetSavingEnabled(bool bEnabled);

	/** ����SaveGame, ������ʧ�����½�(���سɹ�����true, �½�����false)*/
	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadOrCreateSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	void ResetSaveGame();

protected:
	/** ��ǰSave Game���� */
	UPROPERTY()
	USHSaveGame* CurrentSaveGame;

	/** �Ƿ���Ҫ�浵 */
	UPROPERTY()
	bool bSavingEnabled;
};
