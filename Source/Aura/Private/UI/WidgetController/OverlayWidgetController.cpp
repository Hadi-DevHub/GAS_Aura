// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	OnHealthChanged.Broadcast(GetAAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAPS()->OnExperienceChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAPS()->OnLevelChanged.AddLambda([this](int32 NewValue)
	{
		OnPlayerStatChanged.Broadcast(NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (GetAASC())
	{
		GetAASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetAASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfos();
		}
		else
		{
			GetAASC()->AbilityGiven.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfos);
		}
		GetAASC()->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);	
				}
			}
		});
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewValue)
{
	ULevelUpInfo* LevelUpInfo = GetAPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out AuraPlayerState Blueprint"));

	int32 CurrentPlayerLevel = LevelUpInfo->FindLevelForXP(NewValue);
	int32 MaxPlayerLevel = LevelUpInfo->LevelUpInfos.Num() - 1;
	
	if (CurrentPlayerLevel > 0 && CurrentPlayerLevel <= MaxPlayerLevel)
	{
		int32 LevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentPlayerLevel].LevelUpRequirement;
		int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentPlayerLevel - 1].LevelUpRequirement;
		int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;

		float XPBarPercent = static_cast<float>(NewValue) / static_cast<float>(DeltaLevelUpRequirement);
		
		OnPlayerXPChanged.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag= AuraGameplayTags::Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = FGameplayTag();
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
