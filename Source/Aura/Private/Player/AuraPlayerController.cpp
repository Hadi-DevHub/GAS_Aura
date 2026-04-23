// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actors/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"

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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(AuraGameplayTags::PLayer_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetType = ThisActor->Implements<UEnemyInterface>() ? ETargetType::TargetingEnemy : ETargetType::TargetingNonEnemy;
			bAutoMovement = false;
		}
		else
		{
			TargetType = ETargetType::NotTargeting;
		}
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(AuraGameplayTags::PLayer_Block_InputHold))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);	
		return;
	}
	if (TargetType == ETargetType::TargetingEnemy || bShiftAction)
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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(AuraGameplayTags::PLayer_Block_InputReleased))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(AuraGameplayTags::Input_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);	
		return;
	}

	if (bShiftAction) if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if ( TargetType != ETargetType::TargetingEnemy && !bShiftAction )
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
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoMovement = true;
				}
			}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(AuraGameplayTags::Player_Block_CursorTrace))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
		}
		FollowTime = 0.f;
		TargetType = ETargetType::NotTargeting;
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
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial != nullptr)
		{
			MagicCircle->MagicCircleDecal->SetDecalMaterial(DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (MagicCircle)
	{
		FVector MagicCircleLocation = UnderCursor.ImpactPoint;	
		MagicCircleLocation.Z = 0.f;
		MagicCircle->SetActorLocation(MagicCircleLocation);
	}
}

void AAuraPlayerController::HighlightActor(AActor* Actor)
{
	if (IsValid(Actor) && Actor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(Actor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* Actor)
{
	if (IsValid(Actor) && Actor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(Actor);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(AuraGameplayTags::Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, UnderCursor );
	if (!UnderCursor.bBlockingHit) return;
	
	LastActor = ThisActor;
	if (IsValid(UnderCursor.GetActor()) && UnderCursor.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = UnderCursor.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
s		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(AuraGameplayTags::PLayer_Block_InputPressed))
	{
		return;
	}
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

void AAuraPlayerController::ShowDamageText_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlocked, bool bIsCritical)
{
	if (IsValid(TargetCharacter) && DamageTextComponent && IsLocalController())
	{
		UDamageTextComponent* DamageTextComp = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponent);
		DamageTextComp->RegisterComponent();
		DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComp->SetDamageText(DamageAmount, bIsBlocked, bIsCritical);
	}
}
