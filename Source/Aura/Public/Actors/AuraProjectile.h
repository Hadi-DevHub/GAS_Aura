// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;
class USoundBase;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void Destroyed() override;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	bool bIsHit = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ProjectileImpactVFX;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileImpactSFX;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MidAirLoopingSFX;

	TObjectPtr<UAudioComponent> LoopingSFXComponent;
	
};
