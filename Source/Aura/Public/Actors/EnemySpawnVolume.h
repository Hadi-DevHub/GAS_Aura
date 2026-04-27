#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "EnemySpawnVolume.generated.h"

class UBoxComponent;
class AEnemySpawnTargetPoint;

UCLASS()
class AURA_API AEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AEnemySpawnVolume();

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ISaveInterface

	virtual void LoadActor_Implementation() override;

	// End ISaveInterface
	
	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawnTargetPoint*> SpawnTargetPoints;

	UPROPERTY(EditAnywhere, SaveGame, BlueprintReadOnly)
	bool bReached = false;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* SpawnVolume;
	
protected:
	virtual void BeginPlay() override;

};


