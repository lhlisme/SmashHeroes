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

bool USHBlueprintFunctionLibrary::DoesEffectContainerSpecHaveEffects(const FSHGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidEffects();
}

bool USHBlueprintFunctionLibrary::DoesEffectContainerSpecHaveTargets(const FSHGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidTargets();
}

FSHGameplayEffectContainerSpec USHBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(const FSHGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	FSHGameplayEffectContainerSpec NewSpec = ContainerSpec;
	NewSpec.AddTargets(HitResults, TargetActors);
	return NewSpec;
}

TArray<FActiveGameplayEffectHandle> USHBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(const FSHGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of gameplay effects
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		if (SpecHandle.IsValid())
		{
			// If effect is valid, iterate list of targets and apply to all
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}

	return AllEffects;
}

