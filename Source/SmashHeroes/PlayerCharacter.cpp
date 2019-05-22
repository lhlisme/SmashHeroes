// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Engine.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 初始化角色移动控制参数
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bEnableCameraLag = true;

	// Create a camera...
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Speed = FMath::Sqrt(FMath::Square(ForwardInput) + FMath::Square(RightInput));
	if (Speed > 0.0f) {
		IsRunning = true;
	}
	else {
		IsRunning = false;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 设置“移动”绑定
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	// 设置“跳跃”绑定
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);
	// 设置“攻击”绑定
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
}

void APlayerCharacter::MoveForward(float Value)
{
	ForwardInput = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	RightInput = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	bPressedJump = false;
}

void APlayerCharacter::Attack()
{
	if (ComboIndex == 0) {
		++ComboIndex;
		CanCombo = false;
		CanSwitchCombo = false;
	}
	else {
		if (CanCombo) {
			if (CanSwitchCombo) {
				ComboStatus = EComboStatus::ComboSwitched;
			}
			else {
				ComboStatus = EComboStatus::NormalCombo;
			}
			++ComboIndex;
			CanCombo = false;
			CanSwitchCombo = false;
		}
	}
}

void APlayerCharacter::BeginAttack()
{
	Super::BeginAttack();
}

void APlayerCharacter::EndAttack()
{
	Super::EndAttack();
	ComboIndex = 0;		// 攻击结束后重置ComboIndex
}

void APlayerCharacter::BeginEvade()
{
	Super::BeginEvade();
}

void APlayerCharacter::EndEvade()
{
	Super::EndEvade();
	ComboIndex = 0;		// 清空闪避动作时缓存的无效输入
}

void APlayerCharacter::BeginGuard()
{
	Super::BeginGuard();
}

void APlayerCharacter::EndGuard()
{
	Super::EndGuard();
	ComboIndex = 0;		// 清空闪避动作时缓存的无效输入
}

