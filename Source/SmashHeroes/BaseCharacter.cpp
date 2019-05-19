// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create ability system component
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			for (int32 i = 0; i < CharacterAbilities.Num(); ++i)
			{
				if (!CharacterAbilities[i])
				{
					continue;
				}
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(CharacterAbilities[i].GetDefaultObject(), 1, 0));
			}

			for (int32 i = 0; i < CharacterAttributeSets.Num(); ++i)
			{
				AbilitySystem->InitStats(CharacterAttributeSets[i], nullptr);
			}
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes.
// As pawns may be spawned before the client controller possesses them especially in a multiplayer enviroment.
void ABaseCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->RefreshAbilityActorInfo();
}

