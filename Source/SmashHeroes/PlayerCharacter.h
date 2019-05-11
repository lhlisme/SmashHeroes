// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

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
	/** 基础转向速度 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerControl)
	float BaseTurnRate;

	/** 攻击相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	int ComboIndex;		// 当前连击动画索引

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool CanCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = PlayerControl)
	bool ComboSwitch;

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

	// 处理转向的输入
	UFUNCTION()
	void TurnAtRate(float Rate);

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
