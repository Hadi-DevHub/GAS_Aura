#include "AbilitySystem/GameplayAbilities/AuraBeamSpell.h"


void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& DataUnderMouse)
{
	if (DataUnderMouse.bBlockingHit)
	{
		StoredHitActor = DataUnderMouse.GetActor();
		StoredHitLocation = DataUnderMouse.ImpactPoint;
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}
