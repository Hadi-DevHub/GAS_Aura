// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuWidgetController::BroadcastInitialValue()
{
	BroadcastAbilityInfos();
	OnPlayerStatChangedAdded.Broadcast(GetAPS()->GetSpellPoints());
}

void UAuraSpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAPS()->OnSpellPointsAdded.AddLambda([this](int32 NewValue)
	{
		OnPlayerStatChangedAdded.Broadcast(NewValue);
		PendingSpellPoints = NewValue;

		bool bShouldEnableSpendPoints = false;
		bool bShouldEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.StatusTag, NewValue, bShouldEnableSpendPoints, bShouldEnableEquip);
		OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip);
	});
	GetAASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
	{
		if (SelectedAbility.AbilityTag == AbilityTag)
		{
			SelectedAbility.StatusTag = StatusTag;
			
			bool bShouldEnableSpendPoints = false;
			bool bShouldEnableEquip = false;
			ShouldEnableButtons(StatusTag, PendingSpellPoints, bShouldEnableSpendPoints, bShouldEnableEquip);
			OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.AbilityTag = AbilityTag;
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);

			SelectedAbility.StatusTag = StatusTag;
		
		}
	});
}

void UAuraSpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAASC()) GetAASC()->Server_SpendPointButtonPressed_Implementation(SelectedAbility.AbilityTag);
}

void UAuraSpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const bool bIsTagValid = AbilityTag.IsValid();
	const bool bIsTagNone = AbilityTag.MatchesTagExact(AuraGameplayTags::Abilities_Types_None);
	FGameplayTag AbilityStatus = FGameplayTag();

	const int32 SpellPoints = GetAPS()->GetSpellPoints();

	const FGameplayAbilitySpec* AbilitySpec = GetAASC()->GetAbilitySpecFromTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bIsTagValid || bIsTagNone || !bSpecValid)
	{
		AbilityStatus = AuraGameplayTags::Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAASC()->GetStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag; 
	SelectedAbility.StatusTag = AbilityStatus;

	bool bShouldEnableSpendPoints = false;
	bool bShouldEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bShouldEnableSpendPoints, bShouldEnableEquip);
	OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip);
	
}

void UAuraSpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints,
	bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
	bShouldEnableSpendPointsButton = false;
	bShouldEnableEquipButton = false;

	if (StatusTag.MatchesTagExact(AuraGameplayTags::Abilities_Status_Equipped))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
		bShouldEnableEquipButton = true;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags::Abilities_Status_Unlocked))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
		bShouldEnableEquipButton = true;
	}
	else if (StatusTag.MatchesTagExact(AuraGameplayTags::Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
		bShouldEnableEquipButton = false;
	}
	
}
