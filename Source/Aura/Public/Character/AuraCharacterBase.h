// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UAuraDebuffNiagaraComponent;
class UNiagaraSystem;class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() { return AttributeSet; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	//-------------------------------//
	//	COMBAT INTERFACE FUNCTIONS	 //
	//-------------------------------//
	virtual void DIE(const FVector& DeathImpulse) override;
	
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	virtual AActor* GetAvatarActor_Implementation() override;
	virtual bool GetIsDead_Implementation();
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	
	virtual TArray<FTaggedMontages> GetAttackMontages_Implementation() const override;
	virtual FTaggedMontages GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FTaggedMontages> TaggedMontages;
	virtual int32 GetNumberOfMinions_Implementation() const override;
	virtual void IncrementNumberOfMinions_Implementation(int32 Amount) override;

	virtual FOnDeathSignature& GetDelegateToOnDeath() override;
	FOnDeathSignature OnDeathDelegate;
	
	//-------------------------------//
	//								 //
	//-------------------------------//
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsStunned)
    bool bIsStunned = false;
    
    UFUNCTION()
    virtual void OnRep_IsStunned() const;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void AddCharacterAbilities();
	virtual void AddCharacterPassiveAbilities();
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual void OnStunTagChanged(FGameplayTag CallbackTag, int32 NewCount);

	virtual void Dissolve();
	UPROPERTY(EditAnywhere, Category = "Dissolve")
	TObjectPtr<UMaterialInstance> CharacterDissolveMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Dissolve")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterial;

	UFUNCTION(BlueprintImplementableEvent)
	void StartCharacterDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(NetMulticast, reliable)  // This function is related to the DIE function above.
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Combat ")
	float BaseWalkSpeed;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly)
	int32 MinionCount = 0;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterProperties")
	ECharacterClass CharacterClass;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	UPROPERTY(EditAnywhere, Category = "Character Defaults|Attributes")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<UAuraDebuffNiagaraComponent> DebuffNiagaraComponent; 

	virtual FOnAbilitySystemRegistered& DelegateToOnAbilitySystemRegistered() override;
	FOnAbilitySystemRegistered AbilitySystemRegisteredDelegate;

	//-------------------------------//
	//			VFX RELATED			 //
	//-------------------------------//
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults|Animations")
	TObjectPtr<UAnimMontage> HitReactAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults|Animations")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults|DamagedEffect")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults|DamagedEffect")
	TObjectPtr<USoundBase> DeathSound;
};
