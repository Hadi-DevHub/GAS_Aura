// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/ArcaneShards.h"

FString UArcaneShards::GetSpellDescription(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	float Cooldown = GetCooldownDuration(Level);
	float Manacost = GetManaCost(Level);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Arcane>Arcane Shards</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Summons %d Shard(s) of Arcane,</>"
		"<Default> impaling enemies and dealing </><ArcaneDamage>%d </><Default>Arcane Damage in a medium area with a chance to </><ArcaneDamage>KnockBack</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><Manacost> %.2f</>\n"
		"<Default>Cooldown  : </><Cooldown> %.2f</>"
		),
		Level,
		FMath::Min(Level, MaxNumShards),
		CurrentDamage,
		Manacost,
		Cooldown
		);
}

FString UArcaneShards::GetSpellDescriptionNextLevel(int32 Level)
{
	int32 CurrentLevelDamage = Damage.GetValueAtLevel(Level);
	float CurrentCooldown = GetCooldownDuration(Level);
	float CurrentManacost = GetManaCost(Level);
	int32 NextLevelDamage = Damage.GetValueAtLevel(Level + 1);
	float NextCooldown = GetCooldownDuration(Level + 1);
	float NextManacost = GetManaCost(Level + 1);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Arcane>Arcane Shards</>\n"

		// Level
		"<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n"
				
		// Description
		"<Default>Summons</> <PreLeveled>%d </><Default>> %d</> Shard(s) of Arcane,"
		"<Default> impaling enemies and dealing </><PreLeveled>%d </><Default>> </><ArcaneDamage>%d </><Default> Arcane Damage in a medium area with a chance to </><ArcaneDamage>KnockBack</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><PreLeveled>%.2f </><Default>></><Manacost>%.2f</>\n"
		"<Default>Cooldown  : </><PreLeveled>%.2f </><Default>></><Cooldown>%.2f</>"
		),
		Level,
		Level + 1,
		FMath::Min(Level, MaxNumShards),
		FMath::Min(Level + 1, MaxNumShards),
		CurrentLevelDamage,
		NextLevelDamage,
		CurrentManacost,
		NextManacost,
		CurrentCooldown,
		NextCooldown
		);
}
