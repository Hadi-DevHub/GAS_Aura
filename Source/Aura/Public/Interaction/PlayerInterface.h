// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:

	//---------------------//
	//	PLAYER EXPERIENCE  //
	//---------------------//
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToXp(int32 InXP);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayerLevelUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerXP() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 FindLevelForXP(int32 XP);

	//------------------------------//
	//	ATTRIBUTE AND SPELL POINTS  //
	//------------------------------//

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePointsReward(int32 PlayerLevel) const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPointsReward(int32 PlayerLevel) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetAttributePoints() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddToPlayerLevel(int32 InPlayerLevel);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddAttributePoints(int32 AttributePoint);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddSpellPoints(int32 SpellPoint);
	
	//-----------------//
	//	MAGIC CIRCLES  //
	//-----------------//

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();
	
};
