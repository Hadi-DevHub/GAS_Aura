// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Summon Properties")
	float MinSpawnDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summon Properties")
	float MaxSpawnDistance = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Properties")
	float SpawnSpreadAngle = 90.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summon Properties")
	int NumberOfSpawns = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summon Properties")
	TArray<TObjectPtr<AAuraEnemy>> TypesOfEnemiesToSpawn;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
};
