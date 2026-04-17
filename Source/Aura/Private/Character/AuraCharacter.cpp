// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/AuraPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "AbilityStatusNiagara//AuraDebuffNiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameMode/AuraGameModeBase.h"
#include "GameMode/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetUsingAbsoluteRotation(false);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpParticles = CreateDefaultSubobject<UNiagaraComponent>("LevelUpVFXComponent");
	LevelUpParticles->SetupAttachment(GetRootComponent());
	LevelUpParticles->SetAutoActivate(false);
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();

	//TODO : Load Progress
	LoadProgress();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_IsStunned() const
{
	Super::OnRep_IsStunned();
	
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	FGameplayTagContainer BlockedTags;
	BlockedTags.AddTag(AuraGameplayTags::Player_Block_CursorTrace);
	BlockedTags.AddTag(AuraGameplayTags::PLayer_Block_InputHold);
	BlockedTags.AddTag(AuraGameplayTags::PLayer_Block_InputPressed);
	BlockedTags.AddTag(AuraGameplayTags::PLayer_Block_InputReleased);
	
	if (bIsStunned)
	{
		AuraASC->AddLooseGameplayTags(BlockedTags);
		StunDebuffComponent->Activate();
	}
	else
	{
		AuraASC->RemoveLooseGameplayTags(BlockedTags);
		StunDebuffComponent->Deactivate();
	}
}

void AAuraCharacter::OnRep_IsBurned() const
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToXp_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToExperience(InXP);
}

int32 AAuraCharacter::GetPlayerXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerExperience();
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 PlayerLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[PlayerLevel].AttributePointReward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 PlayerLevel) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInfos[PlayerLevel].SpellPointReward;
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 XP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(XP);
}

inline void AAuraCharacter::PlayerLevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::AddAttributePoints_Implementation(int32 AttributePoint)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddAttributePoints(AttributePoint);
}

void AAuraCharacter::AddSpellPoints_Implementation(int32 SpellPoint)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddSpellPoints(SpellPoint);
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddPlayerLevel(InPlayerLevel);
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (AuraASC)
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		PlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		PlayerController->HideMagicCircle();
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(AuraGameMode))
	{
		ULoadScreenSaveGame* GameProgress = AuraGameMode->RetrieveInGameSaveData();
		if (!IsValid(GameProgress)) { return; }
		
		GameProgress->PlayerStartTag = CheckpointTag;
		
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			GameProgress->SavedPlayerLevel = AuraPlayerState->GetPlayerLevel();
			GameProgress->SavedPlayerXP = AuraPlayerState->GetPlayerExperience();
			GameProgress->SavedAttributePoints = AuraPlayerState->GetAttributePoints();
			GameProgress->SavedSpellPoints = AuraPlayerState->GetSpellPoints();
			
			if (UAuraAttributeSet* AAS = Cast<UAuraAttributeSet>(AuraPlayerState->GetAttributeSet()))
			{
				GameProgress->SavedStrength = AAS->GetStrengthAttribute().GetNumericValue(AAS);
				GameProgress->SavedIntelligence = AAS->GetIntelligenceAttribute().GetNumericValue(AAS);
				GameProgress->SavedResilience = AAS->GetResilienceAttribute().GetNumericValue(AAS);
				GameProgress->SavedVigor = AAS->GetVigorAttribute().GetNumericValue(AAS);
			}
		}
		GameProgress->bFirstTimeLoadIn = false;
		AuraGameMode->SaveInGameProgress(GameProgress);
	}
}

void AAuraCharacter::LoadProgress()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(AuraGameMode))
	{
		ULoadScreenSaveGame* GameProgress = AuraGameMode->RetrieveInGameSaveData();
		if (!IsValid(GameProgress)) { return; }

		if (GameProgress->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
			AddCharacterPassiveAbilities();
		}
		else
		{
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
			{
				AuraPlayerState->SetPlayerLevel(GameProgress->SavedPlayerLevel);
				AuraPlayerState->SetPlayerExperience(GameProgress->SavedPlayerXP);
				AuraPlayerState->SetPlayerAttributePoints(GameProgress->SavedAttributePoints);
				AuraPlayerState->SetSpellPoints(GameProgress->SavedSpellPoints);

				UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, GetAbilitySystemComponent(), GameProgress);
			}
			// Todo : Load Progress From Save
		}
	}
}

void AAuraCharacter::OnStunTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	Super::OnStunTagChanged(CallbackTag, NewCount);
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f: BaseWalkSpeed;
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	AbilitySystemRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::OnStunTagChanged);
	if (AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* HUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			HUD->InitOverlay(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (LevelUpParticles)
	{
		FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		FVector LevelUpParticleLocation = LevelUpParticles->GetComponentLocation();
		FRotator FaceCameraRotation = (LevelUpParticleLocation - CameraLocation).Rotation();
		LevelUpParticles->SetWorldRotation(FaceCameraRotation);
		LevelUpParticles->Activate(true);
	}
}
