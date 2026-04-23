// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
#include "UI/Widgets/DamageTextComponent.h"
#include "AuraPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class USplineComponent;
class UAuraAbilitySystemComponent;
class IEnemyInterface;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void ShowDamageText(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlocked, bool bIsCritical);

	//------------------//
	//	 MAGIC CIRCLE	//
	//------------------//

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	UFUNCTION(BlueprintCallable)
	void UpdateMagicCircleLocation();

protected:

	void HighlightActor(AActor* Actor);
	void UnHighlightActor(AActor* Actor);
	
	//------------------//
	//	CLASS DEFAULTS	//
	//------------------//
	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	//------------------//
	//	 MOUSE INPUTS	//
	//------------------//
	
	void AutoRun();
	void CursorTrace();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	//--------------------//
	//	 GAMEPLAY INPUT   //
	//--------------------//

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

	ETargetType TargetType;

	UPROPERTY()
	TObjectPtr<AActor> ThisActor;
	
	UPROPERTY()
	TObjectPtr<AActor> LastActor;

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	// Cursor Trace
	FHitResult UnderCursor;

	// Popping DamageText when damaging enemies

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponent;
	
};


