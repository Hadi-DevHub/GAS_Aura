// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, PlayerXP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

void AAuraPlayerState::AddAttributePoints(int32 Points)
{
	AttributePoints += Points;
	OnAttributePointsAdded.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddSpellPoints(int32 Points)
{
	SpellPoints += Points;
	OnSpellPointsAdded.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddPlayerLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChanged.Broadcast(Level);
}

void AAuraPlayerState::AddToExperience(int32 XP)
{
	PlayerXP += XP;
	OnExperienceChanged.Broadcast(PlayerXP);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level);
}

void AAuraPlayerState::OnRep_PlayerXP(int32 OldXP)
{
	OnExperienceChanged.Broadcast(PlayerXP);
}

void AAuraPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointsAdded.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	OnSpellPointsAdded.Broadcast(SpellPoints);
}
