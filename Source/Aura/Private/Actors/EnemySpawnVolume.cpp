#include "Actors/EnemySpawnVolume.h"

#include "Actors/EnemySpawnTargetPoint.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"

AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>("SpawnVolume");
	SpawnVolume->SetupAttachment(GetRootComponent());
	SpawnVolume->SetCollisionObjectType(ECC_WorldStatic);
	SpawnVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	SpawnVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxOverlap);
}

void AEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bReached = true;
	for (AEnemySpawnTargetPoint* Point : SpawnTargetPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	SpawnVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

