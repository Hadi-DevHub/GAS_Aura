#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){};

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceASC;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;
	
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool bSuccessfulDebuff = false;

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	//------------------//
	//		SETTERS		//
	//------------------//
	
	void SetIsCriticalHit(bool bCritHit) { bIsCriticalHit = bCritHit; }
	void SetIsBlockingHit(bool bHitBlock) { bIsBlockedHit = bHitBlock; }
	void SetIsSuccessfulDebuff(bool bInSuccessfulDebuff) { bSuccessfulDebuff = bInSuccessfulDebuff; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) {	DamageType = InDamageType; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDeathImpulse(FVector InVector) { DeathImpulse = InVector; }
	void SetKnockbackForce(FVector InVector) { KnockbackForce = InVector; }

	//------------------//
	//		GETTERS		//
	//------------------//
	
	bool GetIsCriticalHit() const { return bIsCriticalHit; }
	bool GetIsBlockingHit() const { return bIsBlockedHit; }
	bool GetIsSuccessfulDebuff() const { return bSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency()  const{ return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const 
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const 
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bSuccessfulDebuff = false;
	
	UPROPERTY()
	float DebuffDamage = 0.f;
	
	UPROPERTY()
	float DebuffDuration = 0.f;
	
	UPROPERTY()
	float DebuffFrequency = 0.f; 

	TSharedPtr<FGameplayTag> DamageType;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
		
};