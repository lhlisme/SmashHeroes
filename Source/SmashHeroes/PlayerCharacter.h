// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EComboStatus : uint8
{
	CS_NoCombo				UMETA(DisplayName = "NoCombo"),
	CS_NormalCombo			UMETA(DisplayName = "NormalCombo"),
	CS_ComboSwitched		UMETA(DisplayName = "ComboSwitched")
};

UCLASS(Blueprintable)
class SMASHHEROES_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;
	
	float ForwardInput = 0.f;

	float RightInput = 0.f;

public:
	/** �ƶ�������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool IsRunning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	float Speed = 0.f;

	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	int ComboIndex = 0;		// ��ǰ������������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	EComboStatus ComboStatus = EComboStatus::CS_NoCombo;		// ��ǰ����״̬

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool CanCombo = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool CanSwitchCombo = false;	// �Ƿ���л�����һ����������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool ComboSwitched = false;		// �����������л�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool IsLastCombo = false;	// �Ƿ�Ϊ���������һ������

	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool IsGuarding = false;

	/** ����������� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// ��ɫ���Լ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeSets")
	TArray<TSubclassOf<class UAttributeSet>>	CharacterAttributeSets;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns AbilitySystem subobject **/
	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes
	virtual void PossessedBy(class AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ����ǰ���ƶ�������
	UFUNCTION()
	void MoveForward(float Value);

	// ���������ƶ�������
	UFUNCTION()
	void MoveRight(float Value);

	// ���°���ʱ������Ծ���
	UFUNCTION()
	void StartJump();

	// �ɿ�����ʱ�����Ծ���
	UFUNCTION()
	void StopJump();

	// ���°���ʱ���й���
	UFUNCTION()
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
