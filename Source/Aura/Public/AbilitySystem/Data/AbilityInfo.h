// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UTexture2D> AbilityIcon = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> AbilityBackground = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
    int32 LevelRequirement = 0;
    
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability;
	
};

UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilityInformation;

	UFUNCTION()
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
