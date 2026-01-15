// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/GameplayAbilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FTaggedMontages;
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AbilityProperties")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityProperties")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "AbilityProperties")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 20.f;

	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector RadialDamageOrigin = FVector::ZeroVector;

	UFUNCTION(BlueprintPure)
	FTaggedMontages GetRandomTaggedMontagesFromArray(const TArray<FTaggedMontages>& MontagesTagged) const;
	
	virtual float GetSpellDamage(int32 Level);


};
