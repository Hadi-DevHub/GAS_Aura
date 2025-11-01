// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AblityDamage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AblityDamage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;


};
