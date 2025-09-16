// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	//** Interface Functions **//
	
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	// Combat Interface //
	virtual int32 GetPlayerLevel() override;
	// Combat Interface //
	
	//** Interface Functions **//

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

};
