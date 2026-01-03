#include "AbilitySystem/GameplayAbilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
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
			EDrawDebugTrace::None,
			HitTarget,
			true
			);
		if (HitTarget.bBlockingHit)
		{
			StoredHitActor = HitTarget.GetActor();
			StoredHitLocation = HitTarget.ImpactPoint;
		}
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(StoredHitActor))
		{
			if (!CombatInterface->GetDelegateToOnDeath().IsAlreadyBound(this, &UAuraBeamSpell::OnPrimaryTargetDied))
			{
				CombatInterface->GetDelegateToOnDeath().AddDynamic(this, &UAuraBeamSpell::OnPrimaryTargetDied);
			}
		}
	}
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(StoredHitActor);
	ActorsToIgnore.Add(StoredOwnerAvatar);

	int32 MaxAdditionalTargets = FMath::Min(GetAbilityLevel(), MaxNumOfStoredTargets);
	
	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		StoredHitLocation
		);
	
	UAuraAbilitySystemLibrary::GetClosestTargets(
		MaxAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets,
		StoredHitLocation
		);

	for (auto Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetDelegateToOnDeath().IsAlreadyBound(this, &UAuraBeamSpell::OnAdditionalTargetDied))
			{
				CombatInterface->GetDelegateToOnDeath().AddDynamic(this, &UAuraBeamSpell::OnAdditionalTargetDied);
			}
		}
	}
}


