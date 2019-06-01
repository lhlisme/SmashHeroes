﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	/** 攻击相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	int32 ComboSetIndex = 0;	// 当前连击动画所属集合索引

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	EComboStatus ComboStatus = EComboStatus::NoCombo;		// 当前连击状态

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool CanCombo = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool CanSwitchCombo = false;	// 是否可切换至下一套连击动作

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool ComboSwitched = false;		// 连击动作已切换

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool IsLastCombo = false;	// 是否为连击的最后一个动作

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

	// 处理前后移动的输入
	UFUNCTION()
	void MoveForward(float Value);

	// 处理左右移动的输入
	UFUNCTION()
	void MoveRight(float Value);

	// 按下按键时设置跳跃标记
	UFUNCTION()
	void StartJump();

	// 松开按键时清除跳跃标记
	UFUNCTION()
	void StopJump();

	// 攻击相关
	virtual bool Attack() override;

	virtual void BeginAttack() override;
	
	virtual void EndAttack() override;

	virtual UAnimMontage* GetAttackMontageByIndex() override;

	// 闪避相关
	virtual void BeginEvade() override;

	virtual void EndEvade() override;

	// 防御相关
	virtual void BeginGuard() override;

	virtual void EndGuard() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
