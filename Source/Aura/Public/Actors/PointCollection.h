#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();

	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f) const;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> ImmutablePoints;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_0;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_1;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_2;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_3;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_4;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_5;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_6;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_7;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Pt_8;
	
};
