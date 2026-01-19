// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraElectrocute.h"

FString UAuraElectrocute::GetSpellDescription(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	float Cooldown = GetCooldownDuration(Level);
	float Manacost = GetManaCost(Level);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Lightning>Electrocute</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Emits a beam of lightning,</>"
		"<Default> propagating to %d nearby additional target(s), dealing </><LightningDamage>%d </><Default> rapid lightning damage with a chance to stun at the end</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><Manacost> %.2f</>\n"
		"<Default>Cooldown  : </><Cooldown> %.2f</>"
		),
		Level,
		FMath::Min(Level, MaxNumOfStoredTargets),
		CurrentDamage,
		Manacost,
		Cooldown
		);
}

FString UAuraElectrocute::GetSpellDescriptionNextLevel(int32 Level)
{
	int32 CurrentLevelDamage = Damage.GetValueAtLevel(Level);
	float CurrentCooldown = GetCooldownDuration(Level);
	float CurrentManacost = GetManaCost(Level);
	int32 NextLevelDamage = Damage.GetValueAtLevel(Level + 1);
	float NextCooldown = GetCooldownDuration(Level + 1);
	float NextManacost = GetManaCost(Level + 1);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Lightning>Electrocute</>\n"

		// Level
		"<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n"
					
		// Description
		"<Default>Emits a beam of lightning,</>"
		"<Default> propagating to </><PreLeveled>%d</><Default> ></><LightningDamage> %d</> nearby additional target(s), dealing <PreLeveled>%d </><Default>> </><LightningDamage>%d </><Default> rapid lightning damage with a chance to stun at the end</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><PreLeveled>%.2f </><Default>></><Manacost>%.2f</>\n"
		"<Default>Cooldown  : </><PreLeveled>%.2f </><Default>></><Cooldown>%.2f</>"
		),
		Level,
		Level + 1,
		FMath::Min(Level, MaxNumOfStoredTargets),
		FMath::Min(Level + 1, MaxNumOfStoredTargets),
		CurrentLevelDamage,
		NextLevelDamage,
		CurrentManacost,
		NextManacost,
		CurrentCooldown,
		NextCooldown
		);
}
