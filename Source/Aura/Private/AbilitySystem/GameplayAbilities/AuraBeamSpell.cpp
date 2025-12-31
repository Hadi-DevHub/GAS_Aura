#include "AbilitySystem/GameplayAbilities/AuraBeamSpell.h"

#include "Aura/Aura.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


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

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(StoredOwnerAvatar)
	if (StoredOwnerAvatar && StoredOwnerAvatar->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(StoredOwnerAvatar);
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitTarget;
		UKismetSystemLibrary::SphereTraceSingle(
			StoredOwnerAvatar,
			Weapon->GetSocketLocation(FName("TipSocket")),
			BeamTargetLocation,
			15.f,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitTarget,
			true
			);
		if (HitTarget.bBlockingHit)
		{
			StoredHitActor = HitTarget.GetActor();
			StoredHitLocation = HitTarget.ImpactPoint;
		}
	}
}
