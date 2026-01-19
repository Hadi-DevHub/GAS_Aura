// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actors/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetSpellDescription(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	float Cooldown = GetCooldownDuration(Level);
	float Manacost = GetManaCost(Level);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Fire>Fire Bolt</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Launches %d bolt(s) of fire,</>"
		"<Default> exploding on impact and dealing </><FireDamage>%d </><Default>Fire Damage with a chance to </><FireDamage>Burn</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><Manacost> %.2f</>\n"
		"<Default>Cooldown  : </><Cooldown> %.2f</>"
		),
		Level,
		FMath::Min(Level, NumProjectiles),
		CurrentDamage,
		Manacost,
		Cooldown
		);
}

FString UAuraFireBolt::GetSpellDescriptionNextLevel(int32 Level)
{
	int32 CurrentLevelDamage = Damage.GetValueAtLevel(Level);
	float CurrentCooldown = GetCooldownDuration(Level);
	float CurrentManacost = GetManaCost(Level);
	int32 NextLevelDamage = Damage.GetValueAtLevel(Level + 1);
	float NextCooldown = GetCooldownDuration(Level + 1);
	float NextManacost = GetManaCost(Level + 1);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Fire>Fire Bolt</>\n"

		// Level
		"<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n"
				
		// Description
		"<Default>Launches</> <PreLeveled>%d </><Default>> %d</> bolt(s) of fire,"
		"<Default> exploding on impact and dealing </><PreLeveled>%d </><Default>> </><FireDamage>%d</><Default> Fire Damage with a chance to </><FireDamage>Burn</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><PreLeveled>%.2f </><Default>></><Manacost>%.2f</>\n"
		"<Default>Cooldown  : </><PreLeveled>%.2f </><Default>></><Cooldown>%.2f</>"
		),
		Level,
		Level + 1,
		FMath::Min(Level, NumProjectiles),
		FMath::Min(Level + 1, NumProjectiles),
		CurrentLevelDamage,
		NextLevelDamage,
		CurrentManacost,
		NextManacost,
		CurrentCooldown,
		NextCooldown
		);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(Avatar, SocketTag);
	bool bIsServer = Avatar->HasAuthority();
	if (!bIsServer) return;

	FRotator Forward = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Forward.Pitch = PitchOverride;
	}
	
	int32 NumberOfProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);

	TArray<FRotator> SpawnDirections = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward.Vector(), FVector::UpVector, ProjectileSpread, NumberOfProjectiles);

	for (FRotator Vector : SpawnDirections)
	{
		FTransform SpawnTransform;
    	SpawnTransform.SetLocation(SocketLocation);
    	SpawnTransform.SetRotation(Vector.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//----------------------//
		//  SETTING PROJ PARAMS //
		//----------------------//
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}
