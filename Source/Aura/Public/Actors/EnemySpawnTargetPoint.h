#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnTargetPoint.generated.h"

class AAuraEnemy;
enum class ECharacterClass : uint8;

UCLASS()
class AURA_API AEnemySpawnTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AEnemySpawnTargetPoint();

	void SpawnEnemy();

	UPROPERTY(EditAnywhere, Category = "Spawn Target Point Property")
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawn Target Point Property")
	int32 EnemyLevel;

	UPROPERTY(EditAnywhere, Category = "Spawn Target Point Property")
	ECharacterClass EnemyTypeClass;

protected:
	virtual void BeginPlay() override;

};


