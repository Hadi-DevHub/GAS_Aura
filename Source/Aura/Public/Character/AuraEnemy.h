// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "AuraEnemy.generated.h"

class UAuraDebuffNiagaraComponent;
class UBehaviorTree;
class AAuraAIController;
enum class ECharacterClass : uint8;
class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();
	//-------------------------//
	//   Interface Functions   //
	//-------------------------//
	
	//----------------------//
	//   ENEMY INTERFACE    //
	//----------------------//
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	//----------------------//
	//   COMBAT INTERFACE   //
	//----------------------//
	virtual int32 GetPlayerLevel_Implementation() override;
	
	//------------------------//
	//   Interface Functions  //
	//------------------------//

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/**
	* Reacting to player attacks
	 */

	UPROPERTY(BlueprintReadOnly, Category = " Combat ")
	bool bHitReacting;
	
	UPROPERTY(BlueprintReadOnly, Category = " Combat ")
	bool bOnFire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 3.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	virtual void OnStunTagChanged(FGameplayTag CallbackTag, int32 NewCount) override;

	virtual void DIE(const FVector& DeathImpulse) override;

	UPROPERTY(BlueprintReadOnly)
	bool bHighlighted = false;

	//---------------------------------------//
	//			AI IMPLEMENTATIONS			 //
	//---------------------------------------//

	void PossessedBy(AController* NewController) override;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<AAuraAIController> AuraAIController;

	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	//---------------------------------------//
	//		REACTING TO PLAYER ATTACKS		 //
	//---------------------------------------//
	UFUNCTION()
	void HitReactTagChanged(FGameplayTag CallbackTag, int32 NewCount);
	
};
