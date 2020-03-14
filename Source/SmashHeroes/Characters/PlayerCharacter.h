// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.generated.h"


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

	/** 相机碰撞体积 */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CameraVolume;*/
	
	float ForwardInput = 0.f;

	float RightInput = 0.f;

	/** 防御按键响应计时器 */
	FTimerHandle GuardStartTimerHandle;

public:
	/** 主相机视角 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ViewAngle = -45.0f;

	// 移动相关属性
	/** 输入的移动向量大小 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseControl")
	float MoveInput = 0.0f;

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
	virtual bool MeleeAttack() override;

	// 防御相关
	void TryStartGuardIteratively();

	UFUNCTION(BlueprintCallable)
	void HandleGuardStart();

	UFUNCTION(BlueprintCallable)
	void HandleGuardEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
