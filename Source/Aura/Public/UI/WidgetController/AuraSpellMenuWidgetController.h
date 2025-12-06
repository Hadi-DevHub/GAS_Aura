// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGlobeSelected, bool, bShouldEnableSpendPointButton, bool, bShouldEnableEquipButton);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraSpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FOnGlobeSelected OnGlobeSelected;

private:

	FSelectedAbility SelectedAbility = FSelectedAbility(AuraGameplayTags::Abilities_Types_None, AuraGameplayTags::Abilities_Status_Locked);
	int32 PendingSpellPoints = 0;
	
	UFUNCTION()
	static void ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton);
};
