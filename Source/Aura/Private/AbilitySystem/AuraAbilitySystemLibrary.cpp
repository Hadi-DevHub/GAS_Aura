// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameMode/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(PS->GetAttributeSet());
			const FWidgetControllerParams WCParams(PC, PS, ASC, AS); 
			return AuraHUD->GetOverlayWidgetController(WCParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(UObject* WorldContextObject)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(PS->GetAttributeSet());
			const FWidgetControllerParams WCParams(PC,PS,ASC,AS);
			return HUD->GetAttributeMenuWidgetController(WCParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameModeBase == nullptr) return;
	
	UCharacterClassInfo* CharaClassInfo = AuraGameModeBase->CharacterClassInfo;
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharaClassInfo->GetCharacterClass(CharacterClass);
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();

	AActor* AvatarActor = ASC->GetAvatarActor();
	EffectContextHandle.AddSourceObject(AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandlePrimary = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandlePrimary.Data.Get());

	FGameplayEffectSpecHandle EffectSpecHandleSecondary = ASC->MakeOutgoingSpec(CharaClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandleSecondary.Data.Get());

	FGameplayEffectSpecHandle EffectSpecHandleVitals = ASC->MakeOutgoingSpec(CharaClassInfo->VitalAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandleVitals.Data.Get());
}
