// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/CombatInterface.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, float, EXP);

UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerExperience() const { return PlayerXP; }

	void AddPlayerLevel(int32 InLevel); 
	void AddToExperience(int32 XP); 

	FORCEINLINE void SetPlayerLevel(int32 InLevel) { Level = InLevel; }
	FORCEINLINE void SetToExperience(int32 XP) { PlayerXP = XP; }

	FOnPlayerStatChanged OnExperienceChanged;
	FOnPlayerStatChanged OnLevelChanged;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerXP)
	int PlayerXP;
	UFUNCTION()
	void OnRep_PlayerXP(int32 OldXP);
	
};
