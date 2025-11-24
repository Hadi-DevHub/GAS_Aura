// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTaggedMontages 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSFX = nullptr;
};

class AURA_API ICombatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTarget(const FVector& Direction);

	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent)
	TArray<FTaggedMontages> GetAttackMontages() const;

	virtual void DIE() = 0;

	UFUNCTION(BlueprintNativeEvent)
	bool GetIsDead() const;

	UFUNCTION(BlueprintNativeEvent)
	AActor* GetAvatarActor();

	UFUNCTION(BlueprintNativeEvent)
	UNiagaraSystem* GetBloodEffect() const;

	UFUNCTION(BlueprintNativeEvent)
	FTaggedMontages GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetNumberOfMinions() const;

	UFUNCTION(BlueprintNativeEvent)
	void IncrementNumberOfMinions(int32 Amount);

	UFUNCTION(BlueprintNativeEvent)
	ECharacterClass GetCharacterClass() const;

};
