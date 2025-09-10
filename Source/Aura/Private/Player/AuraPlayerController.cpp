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
#include "Kismet/GameplayStatics.h"

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
	AuraInputComponent->BindAbilityActions(AbilityInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			CachedDestination = Hit.ImpactPoint;
		}
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
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	
		}
		return;
	}
	if (bTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);	
		}
	}
	else
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
					DrawDebugSphere(GetWorld(), PointLoc, 5.f, 8, FColor::Yellow, false, 5.f);
				}
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
