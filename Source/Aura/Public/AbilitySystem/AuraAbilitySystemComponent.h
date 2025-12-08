// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAbilityInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag&, const FGameplayTag& /* Ability Tag */ /* Status Tag */, int32 Level)

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	UFUNCTION()
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);
	void AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& Tag);

	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutDescriptionNextLevel);
	void UpdateAbilityStatuses(int32 Level);
	
	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilityGiven;
	FAbilityStatusChanged AbilityStatusChanged;
	
	bool bStartupAbilitiesGiven = false;

	void UpgradeAttributes(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAttributes(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void Server_SpendPointButtonPressed(const FGameplayTag& AbilityTag);
	
protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatuses(const FGameplayTag& AbilityTag, const  FGameplayTag& StatusTag, int32 Level);

	virtual void OnRep_ActivateAbilities() override;
	
};

