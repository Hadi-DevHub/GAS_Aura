// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* PS = CastChecked<AAuraPlayerState>(PlayerState);

	PS->OnExperienceChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	
	UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilitiesGiven)
		{
			InitializeOnStartupAbilities(AuraASC);
		}
		else
		{
			AuraASC->AbilityGiven.AddUObject(this, &UOverlayWidgetController::InitializeOnStartupAbilities);
		}
		AuraASC->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& AssetTags)
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

void UOverlayWidgetController::InitializeOnStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent) const
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.AbilityTag = AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec);
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewValue)
{
	AAuraPlayerState* PS = CastChecked<AAuraPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo = PS->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, please fill out AuraPlayerState Blueprint"));

	int32 CurrentPlayerLevel = LevelUpInfo->FindLevelForXP(NewValue);
	int32 MaxPlayerLevel = LevelUpInfo->LevelUpInfos.Num() - 1;
	
	if (CurrentPlayerLevel > 0 && CurrentPlayerLevel <= MaxPlayerLevel)
	{
		int32 LevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentPlayerLevel].LevelUpRequirement;
		int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInfos[CurrentPlayerLevel - 1].LevelUpRequirement;
		int32 DeltaLevelUpRequirement = PreviousLevelUpRequirement - LevelUpRequirement;

		float XPBarPercent = static_cast<float>(NewValue) / static_cast<float>(DeltaLevelUpRequirement);
		
		OnPlayerXPChanged.Broadcast(XPBarPercent);
	}
}
