// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraActorEffect.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AAuraActorEffect : public AActor
{
	GENERATED_BODY()
	AAuraActorEffect();
public:	
	

protected:
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<class UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<class UGameplayEffect> InstantGameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	TSubclassOf<class UGameplayEffect> DurationGameplayEffect;
private:


public:
};
 