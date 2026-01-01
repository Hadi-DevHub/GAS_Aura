// Fill out your copyright notice in the Description page of Project Settings.


#include "Debuff/AuraDebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UAuraDebuffNiagaraComponent::UAuraDebuffNiagaraComponent()
{
	bAutoActivate = false;	
}

void UAuraDebuffNiagaraComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
}

void UAuraDebuffNiagaraComponent::Deactivate()
{
	Super::Deactivate();
}

void UAuraDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAuraDebuffNiagaraComponent::OnDebuff_BurnTagChanged);
	}
	else if (GetOwner()->Implements<UCombatInterface>())
	{
		CombatInterface->DelegateToOnAbilitySystemRegistered().AddWeakLambda(this, [this](UAbilitySystemComponent* ASC)
		{
			ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAuraDebuffNiagaraComponent::OnDebuff_BurnTagChanged);
		});
	}
	if (GetOwner()->Implements<UCombatInterface>())
	{
		CombatInterface->GetDelegateToOnDeath().AddDynamic(this, &UAuraDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UAuraDebuffNiagaraComponent::OnDebuff_BurnTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UAuraDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
