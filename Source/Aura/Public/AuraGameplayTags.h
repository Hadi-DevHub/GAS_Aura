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

	// Damage Gameplay Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);

	// Status Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_HitReact);

}