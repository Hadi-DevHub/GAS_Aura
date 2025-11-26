// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32);

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
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; };
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; };

	void AddPlayerLevel(int32 InLevel); 
	void AddToExperience(int32 XP); 

	FORCEINLINE void SetPlayerLevel(int32 InLevel) { Level = InLevel; }
	FORCEINLINE void SetToExperience(int32 XP) { PlayerXP = XP; }

	void AddAttributePoints(int32 Points);
	void AddSpellPoints(int32 Points);

	FOnPlayerStatChanged OnExperienceChanged;
	FOnPlayerStatChanged OnLevelChanged;
	FOnPlayerStatChanged OnAttributePointsAdded;
	FOnPlayerStatChanged OnSpellPointsAdded;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

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
	int32 PlayerXP;
	UFUNCTION()
	void OnRep_PlayerXP(int32 OldXP);

	UPROPERTY(ReplicatedUsing = OnRep_AttributePoint)
	int32 AttributePoints = 0;
	UFUNCTION()
	void OnRep_AttributePoint(int32 OldAttributePoint);

	UPROPERTY(ReplicatedUsing = OnRep_SpellPoint)
	int32 SpellPoints = 0;
	UFUNCTION()
	void OnRep_SpellPoint(int32 OldSpellPoint);
};
