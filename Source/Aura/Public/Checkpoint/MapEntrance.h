// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

	
public:
	
	virtual void Tick(float DeltaTime) override;
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void HighlightActor_Implementation() override;
	virtual void LoadActor_Implementation() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MapDestination;
	
	UPROPERTY(EditAnywhere)
	FName PlayerStartDestination = FName();
};
