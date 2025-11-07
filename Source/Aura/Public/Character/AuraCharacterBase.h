// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

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

	/**
	 *  Derived from ICombatInterface Functions
	 */
	virtual void DIE() override;
	
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;

	virtual AActor* GetAvatarActor_Implementation() override;
	virtual bool GetIsDead() const;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	
	virtual TArray<FTaggedMontages> GetAttackMontages_Implementation() const override;
	virtual FTaggedMontages GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<FTaggedMontages> TaggedMontages;
	
	/**
	 *  Derived from ICombatInterface Functions
	 */
	
protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void AddCharacterAbilities();

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
	virtual void MulticastHandleDeath();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	bool bIsDead = false;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character Defaults|Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	UPROPERTY(EditAnywhere, Category = "Character Defaults|Attributes")
	int32 Level = 1;


	// FX Related
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
