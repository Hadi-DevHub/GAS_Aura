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
	UE_DEFINE_GAMEPLAY_TAG(Input_Passive_1, "Input.Passive.1");
	UE_DEFINE_GAMEPLAY_TAG(Input_Passive_2, "Input.Passive.2");

	// DamageType Gameplay Tags
	// Damage
	UE_DEFINE_GAMEPLAY_TAG(Damage, "Damage");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Fire, "Damage.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Arcane, "Damage.Arcane");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Lightning, "Damage.Lightning");
	UE_DEFINE_GAMEPLAY_TAG(Damage_Physical, "Damage.Physical");

	UE_DEFINE_GAMEPLAY_TAG(Debuff_Burn, "Debuff.Burn");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_Stun, "Debuff.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_Slow, "Debuff.Slow");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_ArmorBreak, "Debuff.ArmorBreak");

	UE_DEFINE_GAMEPLAY_TAG(Debuff_Damage, "Debuff.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_Chance, "Debuff.Chance");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_Duration, "Debuff.Duration");
	UE_DEFINE_GAMEPLAY_TAG(Debuff_Frequency, "Debuff.Frequency");

	// Damage Resistance
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage, "Attributes.Resistance.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Fire, "Attributes.Resistance.Fire");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Arcane, "Attributes.Resistance.Arcane");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Lightning, "Attributes.Resistance.Lightning");
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Resistance_Damage_Physical, "Attributes.Resistance.Physical");

	// Meta Attribute Gameplay Tags
	UE_DEFINE_GAMEPLAY_TAG(Attributes_Meta_IncomingXP, "Attributes.Meta.IncomingXP");

	//---------------------//
	//  ABILITIES SECTION  //
	//---------------------//
	
	// General Abilities Tags
	UE_DEFINE_GAMEPLAY_TAG(Abilities_HitReact, "Abilities.HitReact");

	// Attack Tags
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Attack, "Abilities.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Montage, "Abilities.Summon");

	// Ability Info Tag

	// -- Fire -- //
	UE_DEFINE_GAMEPLAY_TAG(Abilities_None, "Abilities.None");

	// -- Fire -- //
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Fire_Firebolt, "Abilities.Fire.FireBolt");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Fire_Fireblast, "Abilities.Fire.Fireblast");
	
	// -- Lightning -- //
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Lightning_Electrocute, "Abilities.Lightning.Electrocute");
	
	// -- Arcane -- //
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Arcane_ArcaneShards, "Abilities.Arcane.ArcaneShards");

	// -- Passive Abilities -- //
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Passive_HaloOfProtection, "Abilities.Passive.HaloOfProtection");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Passive_HealthSiphon, "Abilities.Passive.HealthSiphon");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Passive_ManaSiphon, "Abilities.Passive.ManaSiphon");

	// Ability Status Tag
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Status_Locked, "Abilities.Status.Locked");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Status_Eligible, "Abilities.Status.Eligible");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Status_Unlocked, "Abilities.Status.Unlocked");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Status_Equipped, "Abilities.Status.Equipped");

	// Ability Type Tag
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Types_Offensive, "Abilities.Types.Offensive");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Types_Passive, "Abilities.Types.Passive");
	UE_DEFINE_GAMEPLAY_TAG(Abilities_Types_None, "Abilities.Types.None");

	// Cooldown Tag
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Fire_Firebolt, "Cooldown.Fire.FireBolt");
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Fire_Fireblast, "Cooldown.Fire.Fireblast");
	UE_DEFINE_GAMEPLAY_TAG(Cooldown_Lightning_Electrocute, "Cooldown.Lightning.Electrocute");
	UE_DEFINE_GAMEPLAY_TAG(	Cooldown_Arcane_ArcaneShards, "Cooldown.Arcane.ArcaneShards");
	
						//*                   *//
						//  MONTAGE SECTION    //
						//*                   *//

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

						//*                  *//
						//	  PLAYER BLOCK    //
						//*                  *//

	UE_DEFINE_GAMEPLAY_TAG(PLayer_Block_InputPressed, "Player.Block.InputPressed");
	UE_DEFINE_GAMEPLAY_TAG(PLayer_Block_InputHold, "Player.Block.InputHold");
	UE_DEFINE_GAMEPLAY_TAG(PLayer_Block_InputReleased, "Player.Block.InputReleased");
	UE_DEFINE_GAMEPLAY_TAG(Player_Block_CursorTrace, "Player.Block.CursorTrace");

	//--------------------//
	//		  CUES	      //
	//--------------------//

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_FireBlast, "GameplayCue.FireBlast");
	
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
	
	DamageTypesToDebuffs.Add(AuraGameplayTags::Damage_Fire, AuraGameplayTags::Debuff_Burn);
	DamageTypesToDebuffs.Add(AuraGameplayTags::Damage_Arcane, AuraGameplayTags::Debuff_Slow);
	DamageTypesToDebuffs.Add(AuraGameplayTags::Damage_Lightning, AuraGameplayTags::Debuff_Stun);
	DamageTypesToDebuffs.Add(AuraGameplayTags::Damage_Physical, AuraGameplayTags::Debuff_ArmorBreak);
}

