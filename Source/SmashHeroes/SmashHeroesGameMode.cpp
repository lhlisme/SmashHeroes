// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SmashHeroesGameMode.h"
#include "SmashHeroesPlayerController.h"
#include "SmashHeroesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASmashHeroesGameMode::ASmashHeroesGameMode()
{
	// use our custom PlayerController class
	//PlayerControllerClass = ASmashHeroesPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}