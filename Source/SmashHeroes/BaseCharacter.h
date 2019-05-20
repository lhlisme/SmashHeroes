// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EArmedState : uint8
{
	DualWield			UMETA(DisplayName = "DualWield"),
	LeftHold			UMETA(DisplayName = "LeftHold"),
	RightHold			UMETA(DisplayName = "RightHold"),
	Unarmed				UMETA(DisplayName = "Unarmed")
};

UCLASS()
class SMASHHEROES_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

public:
	/** 角色武器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AActor>	WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AActor* LeftWeapon;		// 左手武器

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftWeaponSocket;	// 左手武器插槽

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AActor* RightWeapon;	// 右手武器

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RightWeaponSocket;	// 右手武器插槽

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EArmedState ArmedState = EArmedState::Unarmed;

	/** 技能相关属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// 角色属性集
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSets")
	TArray<TSubclassOf<class UAttributeSet>>	CharacterAttributeSets;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns AbilitySystem subobject
	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes
	virtual void PossessedBy(class AController* NewController) override;

	// 生成武器
	void GenerateWeapon();

};
