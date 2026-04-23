#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void HandleGlowEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicInstance);

	// ISaveInterface Inherited Functions

	virtual void LoadActor_Implementation() override;
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	
	// End Of ISaveInterface Inherited Functions

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;
};
