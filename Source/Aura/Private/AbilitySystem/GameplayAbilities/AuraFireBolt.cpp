// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetSpellDescription(int32 Level)
{
	for (auto DamageType : DamageTypes)
	{
		if (DamageType.Key.MatchesTagExact(AuraGameplayTags::Damage_Fire))
		{
			int32 CurrentDamage = DamageType.Value.GetValueAtLevel(Level);
			return FString::Printf(TEXT("<Title_fire>Fire bolt</>\n<Default>Level %d</>\n\n<Default>Launches %d bolt(s) of fire, exploding on impact and dealing </><Damage>%d </><Default>Fire Damage with a chance to burn</>"), Level, Level, CurrentDamage);
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
			int32 NextLevelDamage = DamageType.Value.GetValueAtLevel(Level + 1);
			return FString::Printf(TEXT("<Title_fire>Fire bolt</>\n<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n<Default>Launches %d bolt(s) of fire, exploding on impact and dealing </><Damage>%d </><Default>Fire Damage with a chance to burn</>"), Level, Level + 1, Level + 1, NextLevelDamage);
		}
	}
	return FString();
}
