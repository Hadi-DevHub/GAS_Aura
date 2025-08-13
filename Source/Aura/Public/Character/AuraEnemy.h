// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//** Interface Functions **//
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	//** Interface Functions **//
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
	

};
