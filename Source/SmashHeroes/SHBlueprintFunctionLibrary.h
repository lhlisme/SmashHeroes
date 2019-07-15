// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SHBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ESHLoadingScreenType : uint8
{
	// 游戏开始时使用的加载界面
	StartUpLoadingScreen 				UMETA(DisplayName = "StartUpLoadingScreen"),
	// 切换地图时使用的默认加载界面
	DefaultLoadingScreen				UMETA(DisplayName = "DefaultLoadingScreen")
};

/**
 * 暴露给蓝图的通用函数库
 */
UCLASS()
class SMASHHEROES_API USHBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** 播放加载动画 */
	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void PlayLoadingScreen(ESHLoadingScreenType LoadingScreenType);

	/** 停止加载动画 */
	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void StopLoadingScreen();
};
