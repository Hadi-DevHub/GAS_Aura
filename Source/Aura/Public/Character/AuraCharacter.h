// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;
class AAuraPlayerState;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */

	/* Player Interface */
	virtual void AddToXp_Implementation(int32 InXP) override;
	virtual void PlayerLevelUp_Implementation() override;
	virtual int32 GetPlayerXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) override;
	virtual int32 GetAttributePoints_Implementation(int32 PlayerLevel) const override;
	virtual int32 GetAbilityPoints_Implementation(int32 PlayerLevel) const override;
	virtual void AddAttributePoints_Implementation(int32 AttributePoint) override;
	virtual void AddAbilityPoints_Implementation(int32 AbilityPoint) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	/* End Player Interface */
	
protected:
	
private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> LevelUpParticles;
};

