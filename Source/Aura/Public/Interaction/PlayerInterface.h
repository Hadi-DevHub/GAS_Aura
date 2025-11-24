// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXp(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void PlayerLevelUp();
	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerXP() const;
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 XP);
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints(int32 PlayerLevel) const;
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAbilityPoints(int32 PlayerLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);
	UFUNCTION(BlueprintNativeEvent)
	void AddAttributePoints(int32 AttributePoint);
	UFUNCTION(BlueprintNativeEvent)
	void AddAbilityPoints(int32 AbilityPoint);
	
};
