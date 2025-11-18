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

	// DamageType Gameplay Tags
	// Damage
	UE_DEFINE_GAMEPLAY_TAG(Damage, "Damage");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Fire, "Damage.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Arcane, "Damage.Arcane");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Lightning, "Damage.Lightning");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Physical, "Damage.Physical");

	// Damage Resistance
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage, "Attributes.Resistance.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Fire, "Attributes.Resistance.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Arcane, "Attributes.Resistance.Arcane");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Lightning, "Attributes.Resistance.Lightning");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Physical, "Attributes.Resistance.Physical");

	// Status Tags
	UE_DEFINE_GAMEPLAY_TAG(Status_HitReact, "Status.HitReact");

	// Attack Tags
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack, "Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Montage, "Ability.Summon");

	// Montage Gameplay Tags
	UE_DEFINE_GAMEPLAY_TAG(Montage_Attack_1, "Montage.Attack.1");
	UE_DEFINE_GAMEPLAY_TAG(Montage_Attack_2, "Montage.Attack.2");
	UE_DEFINE_GAMEPLAY_TAG(Montage_Attack_3, "Montage.Attack.3");
	UE_DEFINE_GAMEPLAY_TAG(Montage_Attack_4, "Montage.Attack.4");

	// Combat Socket Tags
	UE_DEFINE_GAMEPLAY_TAG(CombatSocket_Weapon, "CombatSocket.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(CombatSocket_LeftHand, "CombatSocket.LeftHand");
	UE_DEFINE_GAMEPLAY_TAG(CombatSocket_RightHand, "CombatSocket.RightHand");
	UE_DEFINE_GAMEPLAY_TAG(CombatSocket_Tail, "CombatSocket.Tail");

	// Ability Info Tag
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Fire_Firebolt, "Abilities.Fire.FireBolt");
	
	// Cooldown Tag
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Fire_Firebolt, "Cooldown.Fire.FireBolt");
	
}

FAuraGameplayTags& FAuraGameplayTags::Get()
{
	static FAuraGameplayTags Instance;
	return Instance;
}
 
FAuraGameplayTags::FAuraGameplayTags()
{
	InitializeDamageTypeMappings();
}
 
void FAuraGameplayTags::InitializeDamageTypeMappings()
{
	DamageTypesToResistance.Add(AuraGameplayTags::Damage_Fire, AuraGameplayTags::Attributes_Resistance_Damage_Fire);
	DamageTypesToResistance.Add(AuraGameplayTags::Damage_Arcane, AuraGameplayTags::Attributes_Resistance_Damage_Arcane);
	DamageTypesToResistance.Add(AuraGameplayTags::Damage_Lightning, AuraGameplayTags::Attributes_Resistance_Damage_Lightning);
	DamageTypesToResistance.Add(AuraGameplayTags::Damage_Physical, AuraGameplayTags::Attributes_Resistance_Damage_Physical);
}

