// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SHBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ESHLoadingScreenType : uint8
{
	// ��Ϸ��ʼʱʹ�õļ��ؽ���
	StartUpLoadingScreen 				UMETA(DisplayName = "StartUpLoadingScreen"),
	// �л���ͼʱʹ�õ�Ĭ�ϼ��ؽ���
	DefaultLoadingScreen				UMETA(DisplayName = "DefaultLoadingScreen")
};

/**
 * ��¶����ͼ��ͨ�ú�����
 */
UCLASS()
class SMASHHEROES_API USHBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** ���ż��ض��� */
	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void PlayLoadingScreen(ESHLoadingScreenType LoadingScreenType);

	/** ֹͣ���ض��� */
	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void StopLoadingScreen();
};
