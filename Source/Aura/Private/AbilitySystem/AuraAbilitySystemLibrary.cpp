// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameMode/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Interaction/CombatInterface.h"


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
	UCharacterClassInfo* CharaClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharaClassInfo->GetClassDefaultInfo(CharacterClass);
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

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ThisASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharaClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	for (TSubclassOf<UGameplayAbility> Ability : CharaClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		ThisASC->GiveAbility(AbilitySpec);
	}

	FCharacterClassDefaultInfo DefaultInfo = CharaClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> Ability : DefaultInfo.StartupAbilities)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(ThisASC->GetAvatarActor());
		int AbilityLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
		ThisASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	
	return AuraGameMode->CharacterClassInfo;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit( FGameplayEffectContextHandle& Context, bool bIsInBlock)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetIsBlockingHit(bIsInBlock);
	}
}

bool UAuraAbilitySystemLibrary::GetIsBlockedHit(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetIsBlockingHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& Context, bool bIsInCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bIsInCriticalHit);
	}
}

bool UAuraAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetIsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects), FCollisionShape::MakeSphere(radius), SphereParams);
		for (FOverlapResult Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_GetIsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* ThisActor, AActor* OtherActor)
{
	bool bIsThisPlayer = ThisActor->ActorHasTag("Player");
	bool bIsOtherPlayer = OtherActor->ActorHasTag("Player");;

	bool bIsBothPlayer = bIsThisPlayer && bIsOtherPlayer;
	bool bIsBotNotPlayer = !bIsThisPlayer && !bIsOtherPlayer;

	bool bIsFriend = bIsBothPlayer || bIsBotNotPlayer;
	
	return !bIsFriend;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level)
{
	const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = ClassInfo->GetClassDefaultInfo(CharacterClass);
	float XPReward = Info.XPReward.GetValueAtLevel(Level);
	return static_cast<int32>(XPReward);
}
