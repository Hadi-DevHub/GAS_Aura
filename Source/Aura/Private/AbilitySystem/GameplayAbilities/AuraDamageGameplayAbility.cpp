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

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
	FVector DamageOrigin, bool bOverrideKnockBackDirection, FVector InOverrideKnockBackDirection,
	bool bOverrideDeathImpulse, FVector InOverrideDeathImpulse, bool bOverridePitch, float InPitchOverride) const
{
	FDamageEffectParams DamageEffectParams;
	
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;

	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	
	DamageEffectParams.SourceASC = GetAbilitySystemComponentFromActorInfo();
	
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	
	DamageEffectParams.DamageType = DamageType;
	
	DamageEffectParams.DebuffChance = DebuffChance;
	
	DamageEffectParams.DebuffDamage = DebuffDamage;
	
	DamageEffectParams.DebuffDuration = DebuffDuration;
	
	DamageEffectParams.DebuffFrequency = DebuffFrequency;

	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;

	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;

	DamageEffectParams.KnockbackChance = KnockbackChance;

	DamageEffectParams.bIsRadialDamage = bIsRadialDamage;
	if (DamageEffectParams.bIsRadialDamage)
	{
		DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
		DamageEffectParams.RadialDamageOrigin = DamageOrigin;
	}

	if (IsValid(TargetActor))
	{
		DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;

		const FVector ToTarget = Rotation.Vector();
	
		bool bKnockback = FMath::RandRange(0, 100) < DamageEffectParams.KnockbackChance;
		if (bKnockback && !bOverrideKnockBackDirection)
		{
			DamageEffectParams.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		else
		{
			DamageEffectParams.KnockbackForce = FVector::ZeroVector;
		}
		if (!bOverrideDeathImpulse)
		{
			DamageEffectParams.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
	}

	if (bOverrideKnockBackDirection)
	{
		InOverrideKnockBackDirection.Normalize();
		DamageEffectParams.KnockbackForce = InOverrideKnockBackDirection * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = InOverrideKnockBackDirection.Rotation();
			KnockbackRotation.Pitch = InPitchOverride;
			DamageEffectParams.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}

	if (bOverrideDeathImpulse)
	{
		InOverrideDeathImpulse.Normalize();
		DamageEffectParams.DeathImpulse = InOverrideDeathImpulse * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = InOverrideDeathImpulse.Rotation();
			DeathImpulseRotation.Pitch = InPitchOverride;
			DamageEffectParams.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}
	
	return DamageEffectParams;
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


float UAuraDamageGameplayAbility::GetDamageAtLevel()
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
