// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API ACheckpoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void HandleGlowEffects();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicInstance);

private:

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> SphereCollision;
};
