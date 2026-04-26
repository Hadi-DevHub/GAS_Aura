#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bReached = false;

	UPROPERTY(EditAnywhere)
	bool bBindToOverlapCallback = true;
	// Highlight Interface //

	void HighlightActor_Implementation() override;
	void UnHighlightActor_Implementation() override;
	void SetMoveToLocation_Implementation(FVector& OutLocation) override;

	// Highlight Interface //

	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicInstance);

	// ISaveInterface Inherited Functions

	virtual void LoadActor_Implementation() override;
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	
	// End Of ISaveInterface Inherited Functions

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> UniqueMoveToLocation;

	UPROPERTY(EditDefaultsOnly)
	int32 StencilValueOverride = CUSTOM_DEPTH_TAN;

private:

};
