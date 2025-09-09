// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagHeld(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagReleased(InputTag);
}


void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult UnderCursor;
	GetHitResultUnderCursor(ECC_Visibility, false, UnderCursor );
	if (!UnderCursor.bBlockingHit) return;
	
	LastEnemy = ThisEnemy;
	ThisEnemy = UnderCursor.GetActor();

	/**
	Case A
	ThisEnemy = Null, LastEnemy = Null
	DO NOTHING
	
	Case B
	ThisEnemy = Valid, LastEnemy = Null
	ThisEnemy->Highlight()
	 
	Case C
	LastEnemy == Valid, ThisEnemy = Null
	LastEnemy->UnHighlight()
	 
	Case D
	ThisEnemy = Valid, LastEnemy = Valid
	Check --> ThisEnemy = LastEnemy?
	if true, DO NOTHING
	 
	Case E
	ThisEnemy = Valid, LastEnemy = Valid
	Check --> ThisEnemy = LastEnemy?
	if false, ThisEnemy->Highlight(), LastEnemy->UnHighlight()
	
	**/

	if (ThisEnemy != nullptr)
	{
		if (LastEnemy == nullptr)
		{
			ThisEnemy->HighlightActor();
		}
	}
	else
	{
		if (LastEnemy != nullptr)
		{
			LastEnemy->UnHighlightActor();
		}
		else
		{
			// DO NOTHING
		}
	}
	if (ThisEnemy && LastEnemy)
	{
		if (ThisEnemy == LastEnemy)
		{
			// DO NOTHING
		}
		else
		{
			ThisEnemy->HighlightActor();
			LastEnemy->UnHighlightActor();
		}
	}
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, Rotator.Yaw, 0.0f);

	FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = Cast<APawn>(GetPawn()))
	{
		ControlledPawn->AddMovementInput(ForwardDirection * InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection * InputAxisVector.X);
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraASC;
}
