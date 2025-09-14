// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(SHIFTAction, ETriggerEvent::Started, this, &AAuraPlayerController::SHIFTPressed);
	AuraInputComponent->BindAction(SHIFTAction, ETriggerEvent::Completed, this, &AAuraPlayerController::SHIFTReleased);
	AuraInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::SHIFTPressed()
{
	bShiftAction = true;
}

void AAuraPlayerController::SHIFTReleased()
{
	bShiftAction = false;
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		bTargeting = ThisEnemy ? true : false;
		bAutoMovement = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);	
		return;
	}
	if (bTargeting || bShiftAction)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);	
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (UnderCursor.bBlockingHit) CachedDestination = UnderCursor.ImpactPoint;
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector WorldDirection = CachedDestination - ControlledPawn->GetActorLocation();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);	
		return;
	}

	if (bShiftAction) if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);

	if (!bTargeting && !bShiftAction)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn && FollowTime <= ShortPressedThreshold)
		{
			if(UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoMovement = true;
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
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

void AAuraPlayerController::AutoRun()
{
	if (!bAutoMovement) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		FVector ClosestPoint = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		FVector DirectionToPoint = Spline->FindDirectionClosestToWorldLocation(ClosestPoint, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(DirectionToPoint);

		const float DistanceToDestination = (ClosestPoint - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoMovement = false;
		}
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	GEngine->AddOnScreenDebugMessage(1, 0.1, FColor::Black, FString::Printf(TEXT("bSHIFTAction : %d"), bShiftAction));
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, UnderCursor );
	if (!UnderCursor.bBlockingHit) return;
	
	LastEnemy = ThisEnemy;
	ThisEnemy = UnderCursor.GetActor();

	if (LastEnemy != ThisEnemy)
	{
		if (LastEnemy) LastEnemy->UnHighlightActor();
		if (ThisEnemy) ThisEnemy->HighlightActor();
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
	bAutoMovement = false;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraASC;
}
