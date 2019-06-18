// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmashHeroes.h"
#include "GameFramework/Character.h"
#include "UObject/ScriptInterface.h"
#include "AbilitySystemInterface.h"
#include "Abilities/SHAbilitySystemComponent.h"
#include "Abilities/BaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Weapon.h"
#include "BehaviorComponent.h"
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
	RangeAttack			UMETA(DisplayName = "RangeAttack")
};

/** 目标相对方位 */
UENUM(BlueprintType)
enum class ERelativeOrientation : uint8		
{
	Forward				UMETA(DisplayName = "Forward"),
	Backward			UMETA(DisplayName = "Backward"),
	Left				UMETA(DisplayName = "Left"),
	Right				UMETA(DisplayName = "Right")
};

UCLASS()
class SMASHHEROES_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	/** Character所属的Level，在生成时确定，之后不再变化 */
	UPROPERTY(EditAnywhere, Replicated, Category = "Abilities")
	int32 CharacterLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class USHAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCheck", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> LeftDamagedActors;	// 当前攻击左手武器所命中的对象。Key: 被击中的对象; Value; 对象被击中的次数

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCheck", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> RightDamagedActors; // 当前攻击右手武器所命中的对象。Key: 被击中的对象; Value: 对象被击中的次数

	/** If true we have initialized our abilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	int32 bAbilitiesInitialized;

	// 角色行为组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	UBehaviorComponent* BehaviorComponent;

public:
	/** 移动相关属性 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseControl")
	bool IsRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BaseControl")
	float Speed = 0.0f;

	/** 攻击相关属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> MeleeAttackMontageMap;		// 记录近战攻击动画索引和Montage的对应关系 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> RangeAttackMontageMap;		// 记录远程攻击动画索引和Montage的对应关系

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<ERelativeOrientation, UAnimMontage*> HitMontageMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 AttackIndex = 0;		// 当前攻击动画索引

	/** 闪避相关属性 */

	/** 防御相关属性 */

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
	TArray<TSubclassOf<class USHGameplayAbility>> CharacterAbilities;

	// 在创建角色时应用的被动效果，包括初始状态等
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	// 角色属性集
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UBaseAttributeSet* CharacterAttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 计算目标相对朝向
	virtual ERelativeOrientation CalculateRelativeOrientation(AActor* TargetActor);

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
	virtual bool MeleeAttack();

	UFUNCTION(BlueprintCallable)	
	virtual UAnimMontage* GetMeleeAttackMontageByIndex();		// 根据AttackIndex获取当前的近战攻击动画

	UFUNCTION(BlueprintCallable)
	virtual bool RangeAttack();

	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetRangeAttackMontageByIndex();		// 根据AttackIndex获取当前的远程攻击动画

	UFUNCTION(BlueprintCallable)
	void HitPause();								// 受击时定帧效果

	// 闪避相关
	UFUNCTION(BlueprintCallable)
	virtual bool Evade();

	// 防御相关
	UFUNCTION(BlueprintCallable)
	virtual bool Guard();

	UFUNCTION(BlueprintPure)
	UBehaviorComponent* GetBehaviorComponent();

	// 近战攻击检测
	UFUNCTION(BlueprintCallable)
	bool MeleeAttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits);
	
	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const;

	/** 获取血量百分比 */
	UFUNCTION(BlueprintCallable)
	virtual float GetHealthPercentage() const;

	/** Returns current mana */
	UFUNCTION(BlueprintCallable)
	virtual float GetEnergy() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxEnergy() const;

	/** 获取能量百分比 */
	UFUNCTION(BlueprintCallable)
	virtual float GetEnergyPercentage() const;

	/** Returns current movement speed */
	UFUNCTION(BlueprintCallable)
	virtual float GetMoveSpeed() const;

	/** 获取玩家等级信息 */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** 修改玩家等级, 可能改变玩家的能力 */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintPure)
	bool IsAlive();

	/**
	 * 尝试激活指定Tag的所有能力, bAllowRemoteActivation为true时可以远程在服务器上激活能力, 否则只会在本地激活 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	/** 返回指定Tag的已激活(正在运行中)能力 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<USHGameplayAbility*>& ActiveAbilities);

	/** 返回指定Cooldown的总时间和剩余时间。 如果没有激活的Cooldown返回false */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	/** 检查是否能使用指定的能力, ExcludeAbilityTags为与目标能力冲突的所有其他能力的Tags */
	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool CanUseAnyAbility();

	/** 检查是否正在使用指定的能力(注意这里的Tags是“And”关系, 即如果有多个Tag, 将寻找匹配所有Tags的能力) */
	UFUNCTION(BlueprintPure, Category = "Abilities")
	bool IsUsingSpecificAbility(FGameplayTagContainer AbilityTags);

	/**
	 * Called when character takes damage, which may have killed them
	 *
	 * @param DamageAmount Amount of damage that was done, not clamped based on current health
	 * @param HitInfo The hit info that generated this damage
	 * @param DamageTags The gameplay tags of the event that did the damage
	 * @param InstigatorCharacter The character that initiated this damage
	 * @param DamageCauser The actual actor that did the damage, might be a weapon or projectile
	 */
	UFUNCTION(BlueprintCallable)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);

	/**
	 * Called when health is changed, either from healing or from being damaged
	 * For damage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when energy is changed, either from healing or from being used as a cost
	 *
	 * @param DeltaValue Change in energy value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed energy
	 */
	UFUNCTION(BlueprintCallable)
	void OnEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when movement speed is changed
	 *
	 * @param DeltaValue Change in move speed
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintCallable)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** 应用初始化能力和效果 */
	void AddStartupGameplayAbilities();

	/** 移除所有初始能力和效果 */
	void RemoveStartupGameplayAbilities();

	// Called from RPGAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// 将UBaseAttributeSet声明为友元类，使其能访问上述Handle函数
	friend UBaseAttributeSet;
};

