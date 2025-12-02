// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AURA_API UAuraSpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

};
