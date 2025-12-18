// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageEffectHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageEffectHandle, DamageType, ScaledDamage);
	
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

float UAuraDamageGameplayAbility::GetSpellDamage(int32 Level)
{
	float ScaledDamage = Damage.GetValueAtLevel(Level);
	return ScaledDamage;
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;
	
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;

	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	
	DamageEffectParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	
	DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	
	DamageEffectParams.DamageType = DamageType;
	
	DamageEffectParams.DebuffChance = DebuffChance;
	
	DamageEffectParams.DebuffDamage = DebuffDamage;
	
	DamageEffectParams.DebuffDuration = DebuffDuration;
	
	DamageEffectParams.DebuffFrequency = DebuffFrequency;

	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;

	DamageEffectParams.DeathImpulse = FVector::ZeroVector;

	return DamageEffectParams;
}
