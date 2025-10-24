// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties 
{
	GENERATED_BODY()

	FEffectProperties(){};

	FGameplayEffectContextHandle EffectContextHandle;
	
	/**
	 * Source's Data
	 */
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC;
	
	UPROPERTY()
	AActor* SourceAvatarActor;
	
	UPROPERTY()
	AController* SourceController;
	
	UPROPERTY()
	ACharacter* SourceCharacter;
	
	/**
	 * Target's Data
	 */

	UPROPERTY()
	UAbilitySystemComponent* TargetASC;
	
	UPROPERTY()
	AActor* TargetAvatarActor;
	
	UPROPERTY()
	AController* TargetController;
	
	UPROPERTY()
	ACharacter* TargetCharacter;
};

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// A Map to store and used to broadcast values to the AttributeMenuWidgetController
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/**
	 * Primary Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);
	
	/**
	 * Secondary Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary Attribute")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attribute")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category = "Secondary Attribute")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category = "Secondary Attribute")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);

	/**
	 * Secondary Attributes -- Resistances
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category = "Secondary Attribute")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category = "Secondary Attribute")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category = "Secondary Attribute")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Secondary Attribute")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	// -------------------- ON REP SECTIONS ------------------- //
	
	/**
	 * Primary Attributes
	 */

	UFUNCTION()
	void OnRep_Strength(FGameplayAttributeData OldStrength) const;
	
	UFUNCTION()
	void OnRep_Intelligence(FGameplayAttributeData OldIntelligence) const;
	
	UFUNCTION()
	void OnRep_Resilience(FGameplayAttributeData OldResilience) const;
	
	UFUNCTION()
	void OnRep_Vigor(FGameplayAttributeData OldVigor) const;

	/**
	 * Secondary Attributes
	 */
	
	UFUNCTION()
	void OnRep_Armor(FGameplayAttributeData OldStrength) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(FGameplayAttributeData OldStrength) const;
	
	UFUNCTION()
	void OnRep_BlockChance(FGameplayAttributeData OldResilience) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(FGameplayAttributeData OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(FGameplayAttributeData OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(FGameplayAttributeData OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(FGameplayAttributeData OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(FGameplayAttributeData OldManaRegeneration) const;
	
	/**
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	
	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData OldMaxMana) const;

	/**
	 * Secondary Attributes -- Resistances
	 */

	UFUNCTION()
	void OnRep_FireResistance(FGameplayAttributeData OldFireResistance) const;
	
	UFUNCTION()
	void OnRep_ArcaneResistance(FGameplayAttributeData Old_ArcaneResistance) const;
	
	UFUNCTION()
	void OnRep_LightningResistance(FGameplayAttributeData OldLightningResistance) const;
	
	UFUNCTION()
	void OnRep_PhysicalResistance(FGameplayAttributeData OldPhysicalResistance) const;

	/**
	 *	Meta Attributes
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	UFUNCTION()
	void ShowDamageText(FEffectProperties& Props, float DamageAmount, bool bIsBlocked, bool bIsCritical);
	
protected:

private:
	
};


