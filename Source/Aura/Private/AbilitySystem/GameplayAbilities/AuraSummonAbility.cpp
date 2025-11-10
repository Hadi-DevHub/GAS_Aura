// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/AuraSummonAbility.h"

#include "Kismet/GameplayStatics.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	FVector ForwardDirection = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	float DistanceBetweenSpawns = SpawnSpreadAngle / NumberOfSpawns;

	FVector LeftOfSpread = ForwardDirection.RotateAngleAxis(-SpawnSpreadAngle / 2, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int i = 0; i < NumberOfSpawns; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis((DistanceBetweenSpawns * i) + 8.6f, FVector::UpVector);
		FVector SpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance , MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			SpawnLocation + FVector(0.f, 0.f, 400.f),
			SpawnLocation - FVector(0.f, 0.f, 400.f),
			ECC_Visibility
			);

		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint;
			SpawnLocations.Add(SpawnLocation);
		}
	}
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinion()
{
	if (TypesOfEnemiesToSpawn.Num() > 0)
	{
		return TypesOfEnemiesToSpawn[FMath::RandRange(0, TypesOfEnemiesToSpawn.Num() - 1)];
	}
	return nullptr;
}
