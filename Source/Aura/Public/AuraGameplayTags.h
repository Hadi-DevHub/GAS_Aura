// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

/**
 * AuraGameplayTags
 *
 * singleton containing native Gameplay Tags
 */
 
namespace AuraGameplayTags
{
	// Primary
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor);
 
	// Secondary
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana);
 
	// Vitals
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vitals_Health);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Vitals_Mana);

	//Inputs
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_LMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_RMB);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_4);

	// DamageType Gameplay Tags
	// Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

	// Damage Resistance
	// DamageType Gameplay Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Resistance_Damage_Physical);

	// Status Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_HitReact);

}

class FAuraGameplayTags
{
public:
	// Singleton access
	static FAuraGameplayTags& Get();
 
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
 
	void InitializeDamageTypeMappings();
 
private:
	// Private constructor to enforce singleton pattern
	FAuraGameplayTags();
};

