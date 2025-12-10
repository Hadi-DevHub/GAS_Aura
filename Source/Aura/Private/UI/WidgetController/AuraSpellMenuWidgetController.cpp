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
	GetAASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 Level)
	{
		if (SelectedAbility.AbilityTag == AbilityTag)
		{
			SelectedAbility.StatusTag = StatusTag;
			
			bool bShouldEnableSpendPoints = false;
			bool bShouldEnableEquip = false;
			ShouldEnableButtons(StatusTag, PendingSpellPoints, bShouldEnableSpendPoints, bShouldEnableEquip);
			FString Description;
			FString DescriptionNextLevel;
			GetAASC()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
			OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip, Description, DescriptionNextLevel);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	GetAASC()->AbilityEquipped.AddUObject(this, &UAuraSpellMenuWidgetController::OnAbilityEquipped);
	GetAPS()->OnSpellPointsAdded.AddLambda([this](int32 NewValue)
	{
		OnPlayerStatChangedAdded.Broadcast(NewValue);
		PendingSpellPoints = NewValue;

		bool bShouldEnableSpendPoints = false;
		bool bShouldEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.StatusTag, PendingSpellPoints, bShouldEnableSpendPoints, bShouldEnableEquip);
		FString Description;
		FString DescriptionNextLevel;
		GetAASC()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
		OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip, Description, DescriptionNextLevel);
	});
}

void UAuraSpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAASC()) GetAASC()->Server_SpendPointButtonPressed_Implementation(SelectedAbility.AbilityTag);
}

void UAuraSpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bOnWaitingForEquipButtonPressed)
	{
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(AbilityType);
		bOnWaitingForEquipButtonPressed = false;
	}
	
	const bool bIsTagValid = AbilityTag.IsValid();
	const bool bIsTagNone = AbilityTag.MatchesTagExact(AuraGameplayTags::Abilities_Types_None);
	FGameplayTag AbilityStatus = FGameplayTag();

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

	const int32 SpellPoints = GetAPS()->GetSpellPoints();
	bool bShouldEnableSpendPoints = false;
	bool bShouldEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bShouldEnableSpendPoints, bShouldEnableEquip);
	FString Description;
	FString DescriptionNextLevel;
	GetAASC()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, DescriptionNextLevel);
	OnGlobeSelected.Broadcast(bShouldEnableSpendPoints, bShouldEnableEquip, Description, DescriptionNextLevel);
}

void UAuraSpellMenuWidgetController::DeselectSpellGlobe()
{
	if (bOnWaitingForEquipButtonPressed)
	{
		FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(AbilityType);
		bOnWaitingForEquipButtonPressed = false;
	}
	
	SelectedAbility.AbilityTag = AuraGameplayTags::Abilities_Types_None;
	SelectedAbility.StatusTag = AuraGameplayTags::Abilities_Status_Locked;
	OnGlobeSelected.Broadcast(false, false, FString(), FString() );
}

void UAuraSpellMenuWidgetController::EquipButtonPressed()
{
	if (SelectedAbility.AbilityTag.IsValid())
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag);
		StartWaitingForEquipDelegate.Broadcast(Info.AbilityType);
		bOnWaitingForEquipButtonPressed = true;

		FGameplayTag SelectedAbilityStatus = GetAASC()->GetStatusTagFromAbilityTag(SelectedAbility.AbilityTag);
		if (SelectedAbilityStatus.MatchesTagExact(AuraGameplayTags::Abilities_Status_Equipped))
		{
			SelectedGlobeSlot = GetAASC()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
		}
	}
}

void UAuraSpellMenuWidgetController::SpellGlobeRowPressed(const FGameplayTag& SlotType, const FGameplayTag& Slot)
{
	if (!bOnWaitingForEquipButtonPressed) return;

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag);
	if (!Info.AbilityType.MatchesTagExact(SlotType)) return;

	GetAASC()->ServerEquipAbilityToSlot(SelectedAbility.AbilityTag, Slot);
}

void UAuraSpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bOnWaitingForEquipButtonPressed = false;
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag= AuraGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FGameplayTag();
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
}

void UAuraSpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
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
