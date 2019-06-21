// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "SHItem.generated.h"

class USHGameplayAbility;

/** ���е�����Ļ��� */
UCLASS(Abstract, BlueprintType)
class SMASHHEROES_API USHItem : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USHItem()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
		, AbilityLevel(1)
	{}

	/** ��������, �ھ���������������� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;

	/** �������� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	/** �������� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	/** ������ʾͼ�� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FSlateBrush ItemIcon;

	/** ���߼۸� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Price;

	/** ����ʵ�����ӵ�иõ��ߵ�����, <= 0 ��ʾ������ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxCount;

	/** �жϵ����Ƿ�Ϊ����Ʒ(MaxCount <= 0) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Max")
	bool IsConsumable() const;

	/** ���ߵ���ߵȼ�, <= 0 ��ʾ������ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxLevel;

	/** ���߸��������б� */
	typedef TPair<TSubclassOf<USHGameplayAbility>, int32> TGrantedAbilityPair;	// Key: ��������, Value: ����������Ӧ�ȼ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TGrantedAbilityPair> GrantedAbilities;

	/** ��ȡ���ߵ�Ψһ����(Primary Asset ID) */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
