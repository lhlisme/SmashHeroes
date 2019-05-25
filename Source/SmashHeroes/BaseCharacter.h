// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "BaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EArmedState : uint8
{
	DualWield			UMETA(DisplayName = "DualWield"),
	LeftHold			UMETA(DisplayName = "LeftHold"),
	RightHold			UMETA(DisplayName = "RightHold"),
	Unarmed				UMETA(DisplayName = "Unarmed")
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	MeleeAttack			UMETA(DisplayName = "MeleeAttack"),
	RemoteAttack		UMETA(DisplayName = "RemoteAttack")
};

UCLASS()
class SMASHHEROES_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	/** Character所属的Level，在生成时确定，之后不再变化 */
	UPROPERTY(EditAnywhere, Replicated, Category = "Abilities")
	int32 CharacterLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitCheck, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> LeftDamagedActors;	// 当前攻击左手武器所命中的对象。Key: 被击中的对象; Value; 对象被击中的次数

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitCheck, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> RightDamagedActors; // 当前攻击右手武器所命中的对象。Key: 被击中的对象; Value: 对象被击中的次数

public:
	/** 移动相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "BaseControl")
	bool IsRunning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "BaseControl")
	float Speed = 0.f;

	/** 攻击相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool IsAttacking = false;	// TODO 动作被打断时要重置为false

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> AttackMontageMap;		// 记录攻击动画索引和Montage的对应关系 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	int32 ComboIndex = 0;		// 当前攻击动画索引		// TODO 改名AttackIndex

	/** 闪避相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Evade")
	bool IsEvading = false;		// TODO 动作被打断时要重置为false

	/** 防御相关属性 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard")
	bool IsGuarding = false;	// TODO 动作被打断时要重置为false

	/** 角色武器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* LeftWeapon;		// 左手武器网格

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftWeaponSocket;	// 左手武器插槽

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* RightWeapon;	// 右手武器网格

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RightWeaponSocket;	// 右手武器插槽

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EArmedState ArmedState = EArmedState::Unarmed;

	/** 技能相关属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EAttackType AttackType = EAttackType::MeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// 角色属性集
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UAttributeSet>>	CharacterAttributeSets;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void PossessedBy(class AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_Controller() override;	// Make sure that the AbilitySystemComponent's ActorInfo struct is being updated each time the controller changes
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns AbilitySystem subobject
	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	// 生成武器
	void GenerateWeapon();

	bool AddLeftDamagedActor(AActor* CurDamagedActor);

	bool AddRightDamagedActor(AActor* CurDamagedActor);

	UFUNCTION(BlueprintCallable)
	void ClearDamagedActors();

	// 攻击相关
	UFUNCTION(BlueprintCallable)
	virtual bool Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack();

	UFUNCTION(BlueprintCallable)
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable)	
	virtual UAnimMontage* GetAttackMontageByIndex();		// 根据AttackIndex获取当前的连击动画

	// 闪避相关
	UFUNCTION(BlueprintCallable)
	virtual bool Evade();

	UFUNCTION(BlueprintCallable)
	virtual void BeginEvade();

	UFUNCTION(BlueprintCallable)
	virtual void EndEvade();

	// 防御相关
	UFUNCTION(BlueprintCallable)
	virtual bool Guard();

	UFUNCTION(BlueprintCallable)
	virtual void BeginGuard();

	UFUNCTION(BlueprintCallable)
	virtual void EndGuard();

	// 攻击检测
	UFUNCTION(BlueprintCallable)
	bool AttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits);
	
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;
};

