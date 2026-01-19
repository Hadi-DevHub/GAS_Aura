// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	virtual FString GetSpellDescription(int32 Level);
	virtual FString GetSpellDescriptionNextLevel(int32 Level);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ArcaneShards", meta = (AllowPrivateAccess = "true"))
    int32 MaxNumShards = 11;
	
};
