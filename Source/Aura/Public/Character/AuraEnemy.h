// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
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

	/**
	* Reacting to player attacks
	 */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = " Combat ")
	float BaseWalkSpeed;

	UPROPERTY(BlueprintReadOnly, Category = " Combat ")
	bool bHitReacting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	virtual void DIE() override;


	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterProperties")
	ECharacterClass CharacterClass;

	/**
	 * Reacting to player attacks
	 */

	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
