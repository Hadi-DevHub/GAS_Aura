#include "AbilitySystem/GameplayAbilities/AuraBeamSpell.h"
#include "GameFramework/Character.h"


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
		StoredOwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		StoredOwnerAvatar = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}
