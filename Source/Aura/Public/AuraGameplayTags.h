// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags Get() { return GameplayTags; };
	static void InitializeNativeGameplayTags();
protected:

private:
	static FAuraGameplayTags GameplayTags;
};