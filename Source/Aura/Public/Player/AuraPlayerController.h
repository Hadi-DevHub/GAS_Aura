// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "UI/Widgets/DamageTextComponent.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
class IEnemyInterface;
class UInputMappingContext;

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void SetupInputComponent() override;

	// Gameplay Related Function
	UFUNCTION(Client, Reliable)
	void ShowDamageText(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlocked, bool bIsCritical);
protected:
	virtual void BeginPlay() override;
	void AutoRun();
	void CursorTrace();
	virtual void PlayerTick(float DeltaTime) override;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	void Move(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SHIFTAction;
	void SHIFTPressed(); 
	void SHIFTReleased();
	bool bShiftAction = false;

	TScriptInterface<IEnemyInterface> ThisEnemy;
	TScriptInterface<IEnemyInterface> LastEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> AbilityInputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;
	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	float ShortPressedThreshold = .5f;
	bool bAutoMovement = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	bool bTargeting = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// Cursor Trace
	FHitResult UnderCursor;

	// Popping DamageText when damaging enemies

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponent;
};


