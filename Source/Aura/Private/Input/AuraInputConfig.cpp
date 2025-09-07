// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionFromTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Input : AbilityInputActions)
	{
		if (Input.InputAction && Input.ActionTag.MatchesTag(InputTag))
		{
			return Input.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
