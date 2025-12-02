// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"
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
	});
	GetAASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.AbilityTag = AbilityTag;
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}
