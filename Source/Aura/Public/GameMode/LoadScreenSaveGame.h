#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbilities
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> SavedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SavedAbilityTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SavedAbilityStatus = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SavedAbilityType = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SavedAbilityInputTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SavedAbilityLevel = 1;
};


inline bool operator==(const FSavedAbilities& Left, const FSavedAbilities& Right)
{
	return Left.SavedAbilityTag.MatchesTagExact(Right.SavedAbilityTag);
}

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform ActorTransform = FTransform();

	UPROPERTY() // Serialized variables from the Actor - only those marked with SaveGame specifier
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActors& Left, const FSavedActors& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	FName PlayerStartTag = FName();
	
	UPROPERTY()
	FString MapName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus = ESaveSlotStatus::Vacant;

	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	//-- Player --//
	
	UPROPERTY()
	int32 SavedPlayerLevel = 1;

	UPROPERTY()
	int32 SavedPlayerXP = 0;

	UPROPERTY()
	int32 SavedAttributePoints = 0;

	UPROPERTY()
	int32 SavedSpellPoints = 0;

	//-- Player Character's Attributes --//

	UPROPERTY()
	float SavedStrength = 0.f;
	
	UPROPERTY()
	float SavedIntelligence = 0.f;
	
	UPROPERTY()
	float SavedResilience = 0.f;
	
	UPROPERTY()
	float SavedVigor = 0.f;

	//-- Abilities --//
	UPROPERTY()
	TArray<FSavedAbilities> SavedAbilities;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName) const;
	bool HasMap(const FString& InMapName) const;
};
