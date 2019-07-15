// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ILoadingScreenModule.h"
#include "LoadingScreenSettings.h"
#include "SSimpleLoadingScreen.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	FLoadingScreenModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool IsGameModule() const override
	{
		return true;
	}
	virtual void BeginInStartUpLoadingScreen() override;
	virtual void BeginInDefaultLoadingScreen() override;
	virtual void StopInLoadingScreen() override;

private:
	void HandlePrepareLoadingScreen();

	void BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription);
};

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)

FLoadingScreenModule::FLoadingScreenModule()
{

}

void FLoadingScreenModule::StartupModule()
{
	if ( !IsRunningDedicatedServer() && FSlateApplication::IsInitialized() )
	{
		// Load for cooker reference
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		for ( const FStringAssetReference& Ref : Settings->StartupScreen.Images )
		{
			Ref.TryLoad();
		}

		for ( const FStringAssetReference& Ref : Settings->DefaultScreen.Images )
		{
			Ref.TryLoad();
		}

		if ( IsMoviePlayerEnabled() )
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explictly setup the loading screen.
		BeginLoadingScreen(Settings->StartupScreen);
	}
}

void FLoadingScreenModule::ShutdownModule()
{
	if ( !IsRunningDedicatedServer() )
	{
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}
}

void FLoadingScreenModule::HandlePrepareLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->DefaultScreen);
}

void FLoadingScreenModule::BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime = ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable = ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop = ScreenDescription.bWaitForManualStop;
	LoadingScreen.MoviePaths = ScreenDescription.MoviePaths;
	LoadingScreen.PlaybackType = ScreenDescription.PlaybackType;
	
	if ( ScreenDescription.bShowUIOverlay )
	{
		LoadingScreen.WidgetLoadingScreen = SNew(SSimpleLoadingScreen, ScreenDescription);
	}

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void FLoadingScreenModule::BeginInStartUpLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->StartupScreen);
}

void FLoadingScreenModule::BeginInDefaultLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->DefaultScreen);
}

void FLoadingScreenModule::StopInLoadingScreen()
{
	GetMoviePlayer()->StopMovie();
}

#undef LOCTEXT_NAMESPACE
