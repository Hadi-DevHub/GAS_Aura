// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityStatusNiagara/AuraPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UAuraPassiveNiagaraComponent::UAuraPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UAuraPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->PassiveAbilityFXHandle.AddUObject(this, &UAuraPassiveNiagaraComponent::OnReceivePassiveAbilityFXHandle);
		ActivateIfEquipped(AuraASC);
	}
	else
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
		{
			CombatInterface->DelegateToOnAbilitySystemRegistered().AddLambda([this](UAbilitySystemComponent* RegisteredASC)
			{
				if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(RegisteredASC))
				{
					AuraASC->PassiveAbilityFXHandle.AddUObject(this, &UAuraPassiveNiagaraComponent::OnReceivePassiveAbilityFXHandle);
					ActivateIfEquipped(AuraASC);
				}
			}
			);
		}
	}
}

void UAuraPassiveNiagaraComponent::OnReceivePassiveAbilityFXHandle(const FGameplayTag& AbilityTag,
	const bool bShouldActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveAbilityTag))
	{
		if (bShouldActivate)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UAuraPassiveNiagaraComponent::ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC)
{
	const bool bStartupAbilitiesGiven = AuraASC->bStartupAbilitiesGiven;
	if (bStartupAbilitiesGiven)
	{
		if (AuraASC->GetStatusTagFromAbilityTag(PassiveAbilityTag) == AuraGameplayTags::Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
