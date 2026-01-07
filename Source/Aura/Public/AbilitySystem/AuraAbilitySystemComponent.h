// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAbilityInfo;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE(FAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag&, const FGameplayTag& /* Ability Tag */ /* Status Tag */, int32 Level);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& AbilityTag, const FGameplayTag& Status,  const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /* AbilityTag */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FPassiveAbilityFXHandle, const FGameplayTag& /*AbilityTag*/, const bool /*bShould Activate*/);

UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	UFUNCTION()
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);
	void AddCharacterPassiveAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& Tag);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	bool GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutDescriptionNextLevel);
	
	static bool AbilityHasInputTag(const FGameplayAbilitySpec& Spec, const FGameplayTag& InputTag);
	static bool AbilityHasAnyInputTag(const FGameplayAbilitySpec& Spec);
	bool IsThisInputTagEmpty(const FGameplayTag& InputTag);
	FGameplayAbilitySpec* GetAbilitySpecWithInputTag(const FGameplayTag& InputTag);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;
	void AssignInputTagToAbility(const FGameplayTag& InputTag, FGameplayAbilitySpec& AbilitySpec);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbilityToSlot(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PrevSlotTag);

	void ClearAbilitiesForSlot(const FGameplayTag& SlotTag);
	void ClearSlot(FGameplayAbilitySpec* Spec);
	bool AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag);

	void UpdateAbilityStatuses(int32 Level);

	FAbilityEquipped AbilityEquipped;
	FEffectAssetTags EffectAssetTags;
	FAbilityGiven AbilityGiven;
	FAbilityStatusChanged AbilityStatusChanged;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FPassiveAbilityFXHandle PassiveAbilityFXHandle;
	
	bool bStartupAbilitiesGiven = false;

	void UpgradeAttributes(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAttributes(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void Server_SpendPointButtonPressed(const FGameplayTag& AbilityTag);
	
protected:
	UFUNCTION(NetMulticast, Unreliable)
	void NetMulticast_BroadcastHandlePassiveAbilityFX(const FGameplayTag& AbilityTag, const bool bShouldActivate);
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatuses(const FGameplayTag& AbilityTag, const  FGameplayTag& StatusTag, int32 Level);

	virtual void OnRep_ActivateAbilities() override;
	
};

