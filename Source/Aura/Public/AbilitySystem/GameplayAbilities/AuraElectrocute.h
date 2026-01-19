// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()

	virtual FString GetSpellDescription(int32 Level);
	virtual FString GetSpellDescriptionNextLevel(int32 Level);
	
};
