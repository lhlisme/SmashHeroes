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
	/** Character������Level��������ʱȷ����֮���ٱ仯 */
	UPROPERTY(EditAnywhere, Replicated, Category = "Abilities")
	int32 CharacterLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameplayAbilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitCheck, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> LeftDamagedActors;	// ��ǰ�����������������еĶ���Key: �����еĶ���; Value; ���󱻻��еĴ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HitCheck, meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, int32> RightDamagedActors; // ��ǰ�����������������еĶ���Key: �����еĶ���; Value: ���󱻻��еĴ���

public:
	/** �ƶ�������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "BaseControl")
	bool IsRunning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "BaseControl")
	float Speed = 0.f;

	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	bool IsAttacking = false;	// TODO ���������ʱҪ����Ϊfalse

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> AttackMontageMap;		// ��¼��������������Montage�Ķ�Ӧ��ϵ 

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Attack")
	int32 ComboIndex = 0;		// ��ǰ������������		// TODO ����AttackIndex

	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Evade")
	bool IsEvading = false;		// TODO ���������ʱҪ����Ϊfalse

	/** ����������� */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Guard")
	bool IsGuarding = false;	// TODO ���������ʱҪ����Ϊfalse

	/** ��ɫ���� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* LeftWeapon;		// ������������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftWeaponSocket;	// �����������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* RightWeapon;	// ������������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RightWeaponSocket;	// �����������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EArmedState ArmedState = EArmedState::Unarmed;

	/** ����������� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EAttackType AttackType = EAttackType::MeleeAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> CharacterAbilities;

	// ��ɫ���Լ�
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

	// ��������
	void GenerateWeapon();

	bool AddLeftDamagedActor(AActor* CurDamagedActor);

	bool AddRightDamagedActor(AActor* CurDamagedActor);

	UFUNCTION(BlueprintCallable)
	void ClearDamagedActors();

	// �������
	UFUNCTION(BlueprintCallable)
	virtual bool Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual void BeginAttack();

	UFUNCTION(BlueprintCallable)
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable)	
	virtual UAnimMontage* GetAttackMontageByIndex();		// ����AttackIndex��ȡ��ǰ����������

	// �������
	UFUNCTION(BlueprintCallable)
	virtual bool Evade();

	UFUNCTION(BlueprintCallable)
	virtual void BeginEvade();

	UFUNCTION(BlueprintCallable)
	virtual void EndEvade();

	// �������
	UFUNCTION(BlueprintCallable)
	virtual bool Guard();

	UFUNCTION(BlueprintCallable)
	virtual void BeginGuard();

	UFUNCTION(BlueprintCallable)
	virtual void EndGuard();

	// �������
	UFUNCTION(BlueprintCallable)
	bool AttackCheck(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime, TArray<FHitResult>& FinalOutHits);
	
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCharacterLevel() const;
};

