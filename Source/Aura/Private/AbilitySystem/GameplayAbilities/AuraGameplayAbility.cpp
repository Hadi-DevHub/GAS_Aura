// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetSpellDescription(int32 Level)
{
	return FString::Printf(TEXT("<Title>Ability Level :</><Title>%d</>\n\n<Default>LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum"), Level);
}

FString UAuraGameplayAbility::GetSpellDescriptionNextLevel(int32 Level)
{
	return FString::Printf(TEXT("<Default>Level </><Default>%d ></><Default> %d</>\n\n<Default>Causes More Damage!</>"), Level, Level + 1);
}

FString UAuraGameplayAbility::GetSpellLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Title>Ability Locked Until Level %d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel)
{
	float ManaCost = 0.f;
	if (GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Modifier : GetCostGameplayEffect()->Modifiers)
		{
			Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible( InLevel, ManaCost);
			break;
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldownDuration(float InLevel)
{
	float CooldownDuration = 0.f;
	if (GetCooldownGameplayEffect())
	{
		GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownDuration);
	}
	return CooldownDuration;
}
