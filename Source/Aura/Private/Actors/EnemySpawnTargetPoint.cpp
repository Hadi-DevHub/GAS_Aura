#include "Actors/EnemySpawnTargetPoint.h"

#include "Character/AuraEnemy.h"

AEnemySpawnTargetPoint::AEnemySpawnTargetPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemySpawnTargetPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawnTargetPoint::SpawnEnemy()
{
	if (!IsValid(EnemyClass)) { return; }
	
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetUnitLevel(EnemyLevel);
	Enemy->SetUnitClass(EnemyTypeClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}