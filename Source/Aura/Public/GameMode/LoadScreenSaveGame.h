#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
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

	//-- Player --//
	
	UPROPERTY()
	int32 SavedPlayerLevel = 0;

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
};
