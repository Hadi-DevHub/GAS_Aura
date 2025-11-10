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
		SpawnLocations.Add(SpawnLocation);
		
		DrawDebugDirectionalArrow(GetWorld(), Location, Location + Direction * MaxSpawnDistance, 1.f, FColor::Red,false, 5.f );
		DrawDebugSphere(GetWorld(), SpawnLocation, 15.f, 12, FColor::Cyan, false, 5.f );
	}

	return TArray<FVector>();
}
