// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
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

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void Destroyed() override;
	
private:

	// Technical //
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	bool bIsHit = false;

	// Cosmetics //
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ProjectileImpactVFX;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileImpactSFX;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> MidAirLoopingSFX;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSFXComponent;
	
};
