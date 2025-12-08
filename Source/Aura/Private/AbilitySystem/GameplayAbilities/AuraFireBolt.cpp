// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetSpellDescription(int32 Level)
{
	for (auto DamageType : DamageTypes)
	{
		if (DamageType.Key.MatchesTagExact(AuraGameplayTags::Damage_Fire))
		{
			int32 CurrentDamage = GetSpellDamage(Level, AuraGameplayTags::Damage_Fire);
			float Cooldown = GetCooldownDuration(Level);
			float Manacost = GetManaCost(Level);
			return FString::Printf(TEXT(
				// Title 
				"<Title_fire>Fire bolt</>\n"

				// Level
				"<Default>Level %d""</>\n\n"

				// Description
				"<Default>Launches %d bolt(s) of fire,</>"
				"<Default> exploding on impact and dealing </><Damage>%d </><Default>Fire Damage with a chance to burn</>\n\n"

				// Cost and Cooldown
				"<Default>Mana Cost : </><Manacost> %.2f</>\n"
				"<Default>Cooldown  : </><Cooldown> %.2f</>"
				),
				Level,
				FMath::Min(Level, ProjNumber),
				CurrentDamage,
				Manacost,
				Cooldown
				);
		}
	}
	return FString();
}

FString UAuraFireBolt::GetSpellDescriptionNextLevel(int32 Level)
{
	for (auto DamageType : DamageTypes)
	{
		if (DamageType.Key.MatchesTagExact(AuraGameplayTags::Damage_Fire))
		{
			int32 CurrentLevelDamage = DamageType.Value.GetValueAtLevel(Level);
			float CurrentCooldown = GetCooldownDuration(Level);
			float CurrentManacost = GetManaCost(Level);
			int32 NextLevelDamage = DamageType.Value.GetValueAtLevel(Level + 1);
			float NextCooldown = GetCooldownDuration(Level + 1);
			float NextManacost = GetManaCost(Level + 1);
			return FString::Printf(TEXT(
				// Title 
				"<Title_fire>Fire bolt</>\n"

				// Level
				"<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n"
				
				// Description
				"<Default>Launches</> <PreLeveled>%d </><Default>> %d</> bolt(s) of fire,"
				"<Default> exploding on impact and dealing </><Damage>%d </><Default>> </><Damage>%d</><Default> Fire Damage with a chance to burn</>\n\n"

				// Cost and Cooldown
				"<Default>Mana Cost : </><PreLeveled>%.2f </><Default>></><Manacost>%.2f</>\n"
				"<Default>Cooldown  : </><PreLeveled>%.2f </><Default>></><Cooldown>%.2f</>"
				),
				Level,
				Level + 1,
				FMath::Min(Level, ProjNumber),
				FMath::Min(Level + 1, ProjNumber),
				CurrentLevelDamage,
				NextLevelDamage,
				CurrentManacost,
				NextManacost,
				CurrentCooldown,
				NextCooldown
				);
		}
	}
	return FString();
}
