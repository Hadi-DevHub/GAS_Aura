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
