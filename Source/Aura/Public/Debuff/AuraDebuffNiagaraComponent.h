// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraDebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:

	UAuraDebuffNiagaraComponent();
	
	UPROPERTY(EditDefaultsOnly, Category = "Custom Properties")
	FGameplayTag DebuffTag;

	void Activate(bool bReset = false) override;
	
	void Deactivate() override;

	UFUNCTION()
	void OnDebuffTagChanged(FGameplayTag CallbackTag, int32 NewCount);

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);

protected:
	
	virtual void BeginPlay() override;
};
