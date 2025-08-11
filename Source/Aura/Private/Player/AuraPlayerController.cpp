// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

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
