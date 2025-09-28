#include "AbilitySystem/AuraAbilityTypes.h"

bool FAuraGameplayEffectTypes::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
