// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actors/AuraFireBall.h"

FString UAuraFireBlast::GetSpellDescription(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	float Cooldown = GetCooldownDuration(Level);
	float Manacost = GetManaCost(Level);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Fire>Fire Blast</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Summons %d ball(s) of Fire,</>"
		"<Default> launching them in all directions, dealing </><FireDamage>%d </><Default>radial fire damage on contact with a chance to </><FireDamage>Burn</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><Manacost> %.2f</>\n"
		"<Default>Cooldown  : </><Cooldown> %.2f</>"
		),
		Level,
		FMath::Min(Level, MaxNumFireBalls),
		CurrentDamage,
		Manacost,
		Cooldown
		);
}

FString UAuraFireBlast::GetSpellDescriptionNextLevel(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	int32 NextLevelNumMaxFireBalls = Level + 1;
	int32 NextLevelDamage = GetSpellDamage(Level + 1);
	float Cooldown = GetCooldownDuration(Level);
	float NextLevelCooldown = GetManaCost(Level + 1);
	float Manacost = GetManaCost(Level);
	float NextLevelManacost = GetManaCost(Level + 1);
	return FString::Printf(TEXT(
		// Title 
		"<Title_Fire>Fire Blast</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Summons </><PreLeveled>%d</><Default>> %d</> ball(s) of Fire,</>"
		"<Default> launching them in all directions, dealing </><PreLeveled>%d </><Default>> </><FireDamage>%d </><Default>radial fire damage on contact with a chance to </><FireDamage>Burn</><Default>.</>\n\n"

		// Cost and Cooldown
		"<Default>Mana Cost : </><PreLeveled> %.2f</><Default> >%.2f</>\n"
		"<Default>Cooldown  : </><PreLeveled> %.2f</><Default> >%.2f</>"
		),
		Level,
		FMath::Min(Level, MaxNumFireBalls),
		FMath::Min(NextLevelNumMaxFireBalls, MaxNumFireBalls),
		CurrentDamage,
		NextLevelDamage,
		Manacost,
		NextLevelManacost,
		Cooldown,
		NextLevelCooldown
		);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<AAuraFireBall*> FireBalls;
	
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, MaxNumFireBalls);
	for (FRotator Rotator : Rotators)
	{
		FTransform SpawnTransform;
		FVector ActorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		SpawnTransform.SetLocation(ActorLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->FinishSpawning(SpawnTransform);

		FireBalls.Add(FireBall);
	}
	return FireBalls;
}
