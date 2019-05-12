// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EComboStatus : uint8
{
	CS_NoCombo				UMETA(DisplayName = "NoCombo"),
	CS_NormalCombo			UMETA(DisplayName = "NormalCombo"),
	CS_ComboSwitched		UMETA(DisplayName = "ComboSwitched")
};

UCLASS(Blueprintable)
class SMASHHEROES_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	/** 移动相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool IsRunning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	float Speed = 0.f;
	
private:
	float ForwardInput = 0.f;

	float RightInput = 0.f;

public:
	/** 攻击相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	int ComboIndex = 0;		// 当前连击动画索引

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	EComboStatus ComboStatus = EComboStatus::CS_NoCombo;		// 当前连击状态

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool CanCombo = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool CanSwitchCombo = false;	// 是否可切换至下一套连击动作

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool ComboSwitched = false;		// 连击动作已切换

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
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

	// 按下按键时进行攻击
	UFUNCTION()
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
