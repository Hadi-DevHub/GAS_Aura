// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/ExecutionCalculations/ExecCalc_Damage.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	// Secondary Attributes
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	// Damage Type Resistances
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	
	AuraDamageStatics()
	{
		// Secondary Attributes 
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		// Damage Type Resistances
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics GetDamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(GetDamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DeterminedDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs,
	const FGameplayEffectSpec& EffectSpec,
	FAggregatorEvaluateParameters EvaluationParameters) const
{
	for (const TTuple<FGameplayTag, FGameplayTag>& Tag : FAuraGameplayTags::Get().DamageTypesToDebuffs)
	{
		const FGameplayTag DamageType = Tag.Key;
		const FGameplayTag DebuffType = Tag.Value;
		const float TypeDamage = EffectSpec.GetSetByCallerMagnitude(DamageType, false, -1);

		if (TypeDamage > .5f)
		{
			const float SourceDebuffChance = EffectSpec.GetSetByCallerMagnitude(AuraGameplayTags::Debuff_Chance, false, -1);
			float TargetDebuffRes = 0.f;

			const FGameplayTag& ResistanceTag = FAuraGameplayTags::Get().DamageTypesToResistance[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceTag], EvaluationParameters, TargetDebuffRes);
			TargetDebuffRes = FMath::Max<float>(0.f, TargetDebuffRes);
			const float EffectiveDebuffChance = SourceDebuffChance * ((100 - TargetDebuffRes) / 100);
			if (bool bDebuff = FMath::RandRange(0, 100) < EffectiveDebuffChance)
			{
				// TODO: What To DO?
				FGameplayEffectContextHandle EffectHandle = EffectSpec.GetContext();
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(EffectHandle, true);

				FNativeGameplayTag& DebuffDamageTag = AuraGameplayTags::Debuff_Damage;
				const float DebuffDamage = EffectSpec.GetSetByCallerMagnitude(DebuffDamageTag, false, -1);
				FNativeGameplayTag& DebuffDurationTag = AuraGameplayTags::Debuff_Duration;
				const float DebuffDuration = EffectSpec.GetSetByCallerMagnitude(DebuffDurationTag, false, -1);
				FNativeGameplayTag& DebuffFrequencyTag = AuraGameplayTags::Debuff_Frequency;
				const float DebuffFrequency = EffectSpec.GetSetByCallerMagnitude(DebuffFrequencyTag, false, -1);
				
				UAuraAbilitySystemLibrary::SetDebuffDamage(EffectHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(EffectHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(EffectHandle, DebuffFrequency);
				UAuraAbilitySystemLibrary::SetDamageType(EffectHandle, DamageType);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_Armor, GetDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_BlockChance, GetDamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_ArmorPenetration, GetDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitChance, GetDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitDamage, GetDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Secondary_CriticalHitResistance, GetDamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Damage_Fire, GetDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Damage_Arcane, GetDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Damage_Lightning, GetDamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags::Attributes_Resistance_Damage_Physical, GetDamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourceAvatarLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceAvatarLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetAvatarLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetAvatarLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	int Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		FGameplayTag DamageTypeTag = Pair.Key;
		FGameplayTag ResistanceTypeTag = Pair.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("TagsToCaptureDefs doesn't contain tag : [%s] in ExecCalc_Damage"), *ResistanceTypeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[Pair.Value];

		float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(Pair.Key, false);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100 - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Get Block Chance
	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.0f);
	
	// Get Target's Armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.0f);
	
	// Get Source's Armor Penetration
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.0f);

	// Get Critical Hit Chance from source
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.0f);

	// Get Critical Hit Damage from source
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	
	// Get Critical Hit Resistance from target
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.0f);

	//FGameplayEffectContext
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	
										//-----------------------//
										//   DAMAGE CALCULATION  //
										//-----------------------//
	
	// Halves Damage Taken, if Block Chance = true
	const bool bBlockedHit = FMath::FRandRange(0.f,100.f) <= TargetBlockChance;
	Damage = bBlockedHit ? Damage / 2.f : Damage;
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlockedHit);

										//-----------------------//
										//   DEBUFF CALCULATION  //
										//-----------------------//
	 
	DeterminedDebuff(ExecutionParams, TagsToCaptureDefs, EffectSpec, EvaluationParameters);

										//-----------------------//
										//   DEBUFF CALCULATION  //
										//-----------------------//

	// Armor Penetration ignores a percentage of target's armor
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceAvatarLevel);

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetAvatarLevel);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetAvatarLevel);
	
	float EffectiveCriticalChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	EffectiveCriticalChance = FMath::Max<float>(EffectiveCriticalChance, 0.0f);
	
	const bool bCriticalHit = FMath::FRandRange(0.f,100.f) <= EffectiveCriticalChance;
	Damage = bCriticalHit ? Damage += Damage * SourceCriticalHitDamage / 100.f : Damage;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// Effective Armor ignores a percentage of incoming damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
