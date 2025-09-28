#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectTypes : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	void SetIsCriticalhit(bool CritHit) { bIsCriticalHit = CritHit; }
	void SetIsBlockingHit(bool HitBlock) { bIsBlockedHit = HitBlock; }
	
	bool GetIsCriticalHit() const { return bIsCriticalHit; }
	bool GetIsBlockingHit() const { return bIsCriticalHit; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
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

	protected:

	bool bIsBlockedHit = false;

	bool bIsCriticalHit = false;
};
