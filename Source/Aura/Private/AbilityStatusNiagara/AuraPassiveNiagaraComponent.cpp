// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityStatusNiagara/AuraPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
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
