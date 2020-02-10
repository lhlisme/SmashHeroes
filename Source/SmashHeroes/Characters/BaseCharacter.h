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
#include "AI/BehaviorComponent.h"
#include "Base/SHBaseTypes.h"
#include "BaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EArmedState : uint8
{
	DualWield			UMETA(DisplayName = "DualWield"),
	LeftHold			UMETA(DisplayName = "LeftHold"),
	RightHold			UMETA(DisplayName = "RightHold")
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

/** 受击反馈 */
UENUM()
enum class EHitReaction : uint8
{
	BlockHit			UMETA(DisplayName = "BlockHit"),		// 格挡攻击
	HitFront			UMETA(DisplayName = "HitFront"),		// 击中前方
	HitBack				UMETA(DisplayName = "HitBack"),			// 击中后方
	HitLeft				UMETA(DisplayName = "HitLeft"),			// 击中左方
	HitRight			UMETA(DisplayName = "HitRight"),		// 击中右方
	GuardBreak			UMETA(DisplayName = "GuardBreak"),		// 防御击破
	KnockBack			UMETA(DisplayName = "KnockBack"),		// 击退
	KnockUp				UMETA(DisplayName = "KnockUp"),			// 击飞
};

USTRUCT(BlueprintType)
struct FSHAttackMontageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "攻击动画"), Category = "Combo")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "下一连击索引"), Category = "Combo")
	int32 NextComboIndex;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "切换连击索引"), Category = "Combo")
	int32 SwitchComboIndex;
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

	/** 当前攻击动画索引 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	int32 AttackIndex = 0;

	/** 当前攻击动画信息(结构体指针不能于蓝图展示) */
	FSHAttackMontageInfo* AttackInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCheck", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> LeftDamagedActors;	// 当前攻击左手武器所命中的对象。Key: 被击中的对象; Value; 对象被击中的次数

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitCheck", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> RightDamagedActors; // 当前攻击右手武器所命中的对象。Key: 被击中的对象; Value: 对象被击中的次数

	/** 每次受到攻击时角色的受击反馈队列 */
	TArray<EHitReaction> HitReactions;

	/** If true we have initialized our abilities */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	int32 bAbilitiesInitialized;

	// 角色行为组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behavior")
	UBehaviorComponent* BehaviorComponent;

public:
	// 攻击相关属性
	/** 连击索引到Montage间的映射 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, FSHAttackMontageInfo> AttackMontageMap;

	/** 初始(第一段)近战连击索引集合 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<int32> InitMeleeAttacks;

	/** 初始(第一段)远程连击索引集合 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<int32> InitRangeAttacks;

	/** 近战攻击动画索引到Montage间的映射 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> MeleeAttackMontageMap;

	/** 远程攻击动画索引到Montage间的映射 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> RangeAttackMontageMap;

	// 受击相关属性
	/** 普通状态下的受击动画 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit")
	TMap<EHitReaction, UAnimMontage*> HitMontageMap;

	// 闪避相关属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade")
	UAnimMontage* EvadeMontage;		 

	// 防御相关属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guard")
	UAnimMontage* GuardMontage;

	/** 角色武器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	/** 左手武器 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* LeftWeapon;

	/** 左手武器插槽 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftWeaponSocket;	// 左手武器插槽

	/** 左手武器相对位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector LeftRelativeLocation;

	/** 左手武器相对旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FRotator LeftRelativeRotation;

	/** 右手武器 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon* RightWeapon;

	/** 右手武器插槽 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RightWeaponSocket;

	/** 右手武器相对位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector RightRelativeLocation;

	/** 右手武器相对旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FRotator RightRelativeRotation;

	/** 所持武器类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EArmedState ArmedState = EArmedState::DualWield;

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

	/** 生成武器 */
	void GenerateWeapon();

	/** 销毁武器 */
	UFUNCTION(BlueprintCallable)
	void DestroyWeapon();

	bool AddLeftDamagedActor(AActor* CurDamagedActor);

	bool AddRightDamagedActor(AActor* CurDamagedActor);

	UFUNCTION(BlueprintCallable)
	void ClearDamagedActors();

	// 攻击相关
	UFUNCTION(BlueprintCallable)
	virtual bool MeleeAttack();

	/** 根据AttackIndex获取当前的近战攻击动画 */
	UFUNCTION(BlueprintPure)
	virtual UAnimMontage* GetMeleeAttackMontageByIndex();

	UFUNCTION(BlueprintCallable)
	virtual bool RangeAttack();

	/** 根据AttackIndex获取当前的远程攻击动画 */
	UFUNCTION(BlueprintPure)
	virtual UAnimMontage* GetRangeAttackMontageByIndex();

	/** 获取当前攻击动画 */
	UFUNCTION(BlueprintPure)
	virtual UAnimMontage* GetAttackMontage();

	/** 更新目标仇恨值 */
	UFUNCTION(BlueprintCallable)
	void UpdateHateValue(float DamageAmount, AActor* DamageCauser);

	/** 判断目标是否为敌方单位 */
	UFUNCTION(BlueprintPure)
	virtual bool IsTargetHostile(AActor* TargetActor);

	// 受击相关
	/** 添加新的受击反馈(从尾部添加) */
	UFUNCTION(BlueprintCallable)
	void PushHitReaction(EHitReaction NewHitReaction);
	
	/** 移除新的受击反馈(从头部删除) */
	UFUNCTION(BlueprintCallable)
	EHitReaction PopHitReaction();

	/** 计算目标位置相对朝向 */
	ERelativeOrientation CalculateRelativeOrientation(FVector TargetLocation);

	/** 检测受击点是否处于防御范围内, 并判断受击点的相对方位 */
	bool IsHitInDefenseRange(FVector HitLocation, EHitReaction& HitReaction);

	/** 计算攻击是否被格挡掉、格挡后的最终伤害和对应的受击反馈 */
	UFUNCTION(BlueprintCallable)
	void CheckHitResult(FVector HitLocation, float DefenseFactor, float& DamageDone, float& EnergyCost);

	// 闪避相关
	UFUNCTION(BlueprintCallable)
	virtual bool Evade();

	UFUNCTION(BlueprintPure)
	virtual UAnimMontage* GetEvadeMontage();

	// 防御相关
	UFUNCTION(BlueprintCallable)
	virtual bool Guard();

	UFUNCTION(BlueprintPure)
	virtual UAnimMontage* GetGuardMontage();

	UFUNCTION(BlueprintPure)
	UBehaviorComponent* GetBehaviorComponent() const;

	UFUNCTION(BlueprintPure)
	EBehaviorType GetCurrentBehavior();

	// 近战攻击检测
	UFUNCTION(BlueprintCallable)
	bool MeleeAttackCheck(const EAttackStrength AttackStrength, const bool CheckLeft, const bool CheckRight, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits, FGameplayAbilityTargetDataHandle& HitTargetsData);
	
	/** Returns current health, will be 0 if dead */
	UFUNCTION(BlueprintPure)
	virtual float GetHealth() const;

	/** Returns maximum health, health will never be greater than this */
	UFUNCTION(BlueprintPure)
	virtual float GetMaxHealth() const;

	/** 获取血量百分比 */
	UFUNCTION(BlueprintPure)
	virtual float GetHealthPercentage() const;

	/** Returns current mana */
	UFUNCTION(BlueprintPure)
	virtual float GetEnergy() const;

	/** Returns maximum mana, mana will never be greater than this */
	UFUNCTION(BlueprintPure)
	virtual float GetMaxEnergy() const;

	/** 获取能量百分比 */
	UFUNCTION(BlueprintPure)
	virtual float GetEnergyPercentage() const;

	/** 获取当前移动速度 */
	UFUNCTION(BlueprintPure)
	virtual float GetMoveSpeed() const;

	/** 获取当前防御范围(角度) */
	UFUNCTION(BlueprintPure)
	virtual float GetDefenseRange() const;

	/** 获取玩家等级信息 */
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;

	/** 修改玩家等级, 可能改变玩家的能力 */
	UFUNCTION(BlueprintCallable)
	virtual bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintPure)
	bool IsAlive();

	// 特效相关
	/** 播放受击特效 */
	UFUNCTION(BlueprintCallable)
	void PlayEmissiveEffect(FLinearColor EmissiveEffectColor);

	/** 播放死亡溶解特效 */
	UFUNCTION(BlueprintCallable)
	void PlayDissolveEffect(float DissolveLength, FLinearColor DissolveColor);

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
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);

	/**
	 * Called when health is changed, either from healing or from being damaged
	 * For damage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when energy is changed, either from healing or from being used as a cost
	 *
	 * @param DeltaValue Change in energy value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed energy
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/**
	 * Called when movement speed is changed
	 *
	 * @param DeltaValue Change in move speed
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	/** 死亡时回调 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();

	/** 应用初始化能力和效果 */
	void AddStartupGameplayAbilities();

	/** 移除所有初始能力和效果 */
	void RemoveStartupGameplayAbilities();

	/** 受击处理 */
	UFUNCTION(BlueprintCallable)
	virtual void HandleHit(float DamageAmount, AActor* DamageCauser, FLinearColor InLinearColor);

	UFUNCTION(BlueprintCallable)
	virtual void HandleDead();

	// Called from BaseAttributeSet, these call BP events above
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, ABaseCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleEnergyChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);


	// 将UBaseAttributeSet声明为友元类，使其能访问下述Handle函数
	friend UBaseAttributeSet;
};

