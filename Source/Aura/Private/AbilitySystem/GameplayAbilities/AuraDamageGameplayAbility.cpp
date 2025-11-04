// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageEffectHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	for (TTuple<FGameplayTag, FScalableFloat> DamageType : DamageTypes)
	{
		const float ScaledDamage = DamageType.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectHandle, DamageType.Key, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageEffectHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontages UAuraDamageGameplayAbility::GetRandomTaggedMontagesFromArray(const TArray<FTaggedMontages>& MontagesTagged) const
{
	if (MontagesTagged.Num() > 0)
	{
		return MontagesTagged[FMath::RandRange(0, MontagesTagged.Num() - 1)];
	}
	return FTaggedMontages();
}
