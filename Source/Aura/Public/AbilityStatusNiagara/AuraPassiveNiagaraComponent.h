// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraPassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UAuraPassiveNiagaraComponent();
	
protected:
	
	virtual void BeginPlay() override;

private:
		
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAbilityTag;

	UFUNCTION()
	void OnReceivePassiveAbilityFXHandle(const FGameplayTag& AbilityTag, const bool bShouldActivate);

	UFUNCTION()
	void ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC);
};
