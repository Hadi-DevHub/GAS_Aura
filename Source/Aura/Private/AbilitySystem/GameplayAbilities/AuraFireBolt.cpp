// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"

FString UAuraFireBolt::GetSpellDescription(int32 Level)
{
	int32 CurrentDamage = GetSpellDamage(Level);
	float Cooldown = GetCooldownDuration(Level);
	float Manacost = GetManaCost(Level);
	return FString::Printf(TEXT(
		// Title 
		"<Title_fire>Fire bolt</>\n"

		// Level
		"<Default>Level %d""</>\n\n"

		// Description
		"<Default>Launches %d bolt(s) of fire,</>"
		"<Default> exploding on impact and dealing </><Damage>%d </><Default>Fire Damage with a chance to burn</>\n\n"

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
		"<Title_fire>Fire bolt</>\n"

		// Level
		"<Default>Level </><PreLeveled>%d > </><Level>%d</>\n\n"
				
		// Description
		"<Default>Launches</> <PreLeveled>%d </><Default>> %d</> bolt(s) of fire,"
		"<Default> exploding on impact and dealing </><Damage>%d </><Default>> </><Damage>%d</><Default> Fire Damage with a chance to burn</>\n\n"

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

void UAuraFireBolt::SpawnProjectiles(const FVector& SpawnLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	bool bIsServer = Avatar->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	int32 NumberOfProjectiles = FMath::Min(NumProjectiles, MaxNumProjectiles);
	
	FVector ForwardRotation = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector LeftOfSpread = ForwardRotation.RotateAngleAxis(-ProjectileSpread / 2, FVector::UpVector);
	FVector RightOfSpread = ForwardRotation.RotateAngleAxis(ProjectileSpread / 2, FVector::UpVector);
	
	UKismetSystemLibrary::DrawDebugArrow(Avatar, SocketLocation, SocketLocation + ForwardRotation * 75.f, 1.f, FColor::White, 20.f, 1.f );
	UKismetSystemLibrary::DrawDebugArrow(Avatar, SocketLocation, SocketLocation + LeftOfSpread * 75.f, 1.f, FColor::White, 20.f, 1.f );
	UKismetSystemLibrary::DrawDebugArrow(Avatar, SocketLocation, SocketLocation + RightOfSpread * 75.f, 1.f, FColor::White, 20.f, 1.f );

	if (NumberOfProjectiles > 1)
	{
		const float DeltaRotation = ProjectileSpread / (NumberOfProjectiles - 1);
		for (int32 i = 0; i < NumberOfProjectiles; i++)
		{
			FVector Start = SocketLocation + FVector(0,0,10.f);
			FVector ProjectileDirection = LeftOfSpread.RotateAngleAxis(DeltaRotation * i, FVector::UpVector);
			FVector End   = Start + ProjectileDirection * 50.f;
			
			UKismetSystemLibrary::DrawDebugArrow(Avatar,
				Start,
				End,
				1.f,
				FColor::Red,
				20.f,
				1.f
				);
		}
	}

}
