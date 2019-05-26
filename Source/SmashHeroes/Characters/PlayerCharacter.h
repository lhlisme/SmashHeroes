// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EComboStatus : uint8
{
	NoCombo				UMETA(DisplayName = "NoCombo"),
	NormalCombo			UMETA(DisplayName = "NormalCombo"),
	ComboSwitched		UMETA(DisplayName = "ComboSwitched")
};

UCLASS(Blueprintable)
class SMASHHEROES_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	float ForwardInput = 0.f;

	float RightInput = 0.f;

public:
	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	int32 ComboSetIndex = 0;	// ��ǰ��������������������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	EComboStatus ComboStatus = EComboStatus::NoCombo;		// ��ǰ����״̬

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool CanCombo = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool CanSwitchCombo = false;	// �Ƿ���л�����һ����������

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool ComboSwitched = false;		// �����������л�

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool IsLastCombo = false;	// �Ƿ�Ϊ���������һ������

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

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

	// �������
	virtual bool Attack() override;

	virtual void BeginAttack() override;
	
	virtual void EndAttack() override;

	virtual UAnimMontage* GetAttackMontageByIndex() override;

	// �������
	virtual void BeginEvade() override;

	virtual void EndEvade() override;

	// �������
	virtual void BeginGuard() override;

	virtual void EndGuard() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
