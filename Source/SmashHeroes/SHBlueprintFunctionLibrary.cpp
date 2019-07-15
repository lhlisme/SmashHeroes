// Fill out your copyright notice in the Description page of Project Settings.


#include "SHBlueprintFunctionLibrary.h"
#include "ILoadingScreenModule.h"


void USHBlueprintFunctionLibrary::PlayLoadingScreen(ESHLoadingScreenType LoadingScreenType)
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();

	switch (LoadingScreenType)
	{
	case ESHLoadingScreenType::StartUpLoadingScreen:
		LoadingScreenModule.BeginInStartUpLoadingScreen();
		break;
	case ESHLoadingScreenType::DefaultLoadingScreen:
		LoadingScreenModule.BeginInDefaultLoadingScreen();
		break;
	default:
		break;
	}
}

void USHBlueprintFunctionLibrary::StopLoadingScreen()
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
}

