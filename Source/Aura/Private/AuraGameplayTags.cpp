// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

namespace AuraGameplayTags
{
 
	// Primary
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Strength, "Attributes.Primary.Strength");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Intelligence, "Attributes.Primary.Intelligence");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Resilience, "Attributes.Primary.Resilience");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Primary_Vigor, "Attributes.Primary.Vigor");
 
	// Secondary
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_Armor, "Attributes.Secondary.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ArmorPenetration, "Attributes.Secondary.ArmorPenetration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_BlockChance, "Attributes.Secondary.BlockChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitChance, "Attributes.Secondary.CriticalHitChance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitDamage, "Attributes.Secondary.CriticalHitDamage");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_CriticalHitResistance, "Attributes.Secondary.CriticalHitResistance");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_HealthRegeneration, "Attributes.Secondary.HealthRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_ManaRegeneration, "Attributes.Secondary.ManaRegeneration");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxHealth, "Attributes.Secondary.MaxHealth");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Secondary_MaxMana, "Attributes.Secondary.MaxMana");
 
	// Vitals
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vitals_Health, "Attributes.Vitals.Health");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Vitals_Mana, "Attributes.Vitals.Mana");

	// Input
	UE_DEFINE_GAMEPLAY_TAG(Input_LMB, "Input.LMB");
	UE_DEFINE_GAMEPLAY_TAG(Input_RMB, "Input.RMB");
	UE_DEFINE_GAMEPLAY_TAG(Input_1, "Input.1");
	UE_DEFINE_GAMEPLAY_TAG(Input_2, "Input.2");
	UE_DEFINE_GAMEPLAY_TAG(Input_3, "Input.3");
	UE_DEFINE_GAMEPLAY_TAG(Input_4, "Input.4");

	// Damage Gameplay Tags
	UE_DEFINE_GAMEPLAY_TAG(Damage, "Damage");
	
	// Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Status_HitReact, "Status.HitReact");
	
}