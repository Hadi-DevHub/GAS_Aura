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
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Passive_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Passive_2);

	// DamageType Gameplay Tags
	// Damage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Slow);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_ArmorBreak);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Duration);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Frequency);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Chance);

	// Damage Resistance
	// DamageType Gameplay Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Damage_Arcane);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Damage_Lightning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Damage_Physical);

	// Meta Attribute Gameplay Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Meta_IncomingXP);
	
	//---------------------//
	//  ABILITIES SECTION  //
	//---------------------//
	
	// Attack Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon);
	
	// Ability Status Tag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Locked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Eligible);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Unlocked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Status_Equipped);

	// Ability Type Tag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Types_Offensive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Types_Passive);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Types_None);
	
	// Ability Element Type
	
	//-- Fire -- //
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Fire_FireBolt);
	
	// -- Lightning -- //
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Lightning_Electrocute);
	
	// -- Arcane -- //
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Arcane_ArcaneShards);


	// -- Passive Abilities -- //
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_HaloOfProtection);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_HealthSiphon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Passive_ManaSiphon);
	
	// Status Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_HitReact);

	//*							   *//
	//  ABILITIES COOLDOWN SECTION  //
	//*						       *//
		
	// Cooldown Tag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Fire_Firebolt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Lightning_Electrocute);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooldown_Arcane_ArcaneShards);


	//*                   *//
	//  MONTAGES SECTION   //
	//*                   *//
	
	// Montage Gameplay Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

	//Combat Socket Tags
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail);
	
	//--------------------//
	//	  PLAYER BLOCK    //
	//--------------------//

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PLayer_Block_InputPressed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PLayer_Block_InputHold);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PLayer_Block_InputReleased);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Block_CursorTrace);

}

class FAuraGameplayTags
{
public:
	// Singleton access
	static FAuraGameplayTags& Get();
 
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
 
	void InitializeDamageTypeMappings();
 
private:
	// Private constructor to enforce singleton pattern
	FAuraGameplayTags();
};

