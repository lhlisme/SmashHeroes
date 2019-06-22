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
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 创建相机臂
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1600.0f;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bEnableCameraLag = true;

	// 创建相机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 创建相机碰撞体积
	/*CameraVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraCollision"));
	CameraVolume->SetupAttachment(FollowCamera);
	CameraVolume->SetBoxExtent(FVector(45.0f, 22.0f, 22.0f), true);
	CameraVolume->SetCollisionResponseToAllChannels(ECR_Overlap);*/
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorComponent) {
		// 添加攻击状态初始化回调事件
		BehaviorComponent->OnMeleeAttackEnd.AddDynamic(this, &APlayerCharacter::ResetAttackStatus);
		BehaviorComponent->OnEvadeBegin.AddDynamic(this, &APlayerCharacter::ResetAttackStatus);
		BehaviorComponent->OnGuardBegin.AddDynamic(this, &APlayerCharacter::ResetAttackStatus);
	}

	// 初始化相机臂朝向
	CameraBoom->SetWorldRotation(FRotator(ViewAngle, GetActorRotation().Yaw, 0.0f));
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
	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
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

bool APlayerCharacter::MeleeAttack()
{
	if (AttackIndex == 0) {
		++AttackIndex;
		CanCombo = false;
		CanSwitchCombo = false;
		return true;
	}
	else {
		if (CanCombo) {
			if (CanSwitchCombo) {
				ComboStatus = EComboStatus::ComboSwitched;
				ComboSetIndex = AttackIndex;
			}
			else {
				ComboStatus = EComboStatus::NormalCombo;
			}
			++AttackIndex;
			CanCombo = false;
			CanSwitchCombo = false;
			return true;
		}
	}

	return false;
}

UAnimMontage* APlayerCharacter::GetMeleeAttackMontageByIndex()
{
	int Index = AttackIndex + ComboSetIndex * 10;

	UE_LOG(LogTemp, Log, TEXT("Index: %d"), Index);
	UAnimMontage** CurAttackMontagePtr = MeleeAttackMontageMap.Find(Index);

	if (CurAttackMontagePtr) {
		return *CurAttackMontagePtr;
	}

	return nullptr;
}

void APlayerCharacter::ResetAttackStatus()
{
	UE_LOG(LogTemp, Log, TEXT("ResetAttackStatus"));
	// 如果攻击结束或被中断的话, 重新初始化AttackIndex
	AttackIndex = 0;
	ComboSetIndex = 0;
}

