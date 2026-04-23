// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widgets/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(GetWorld(), AbilitySystemComponent, CharacterClass);
	}
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraAS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& data)
			{
				OnHealthChanged.Broadcast(data.NewValue);
			}
			);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& data)
			{
				OnMaxHealthChanged.Broadcast(data.NewValue);
			}
			);
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
		AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::Abilities_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraEnemy::HitReactTagChanged);
		AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags::Debuff_Stun).AddUObject(this, &AAuraEnemy::OnStunTagChanged);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	AbilitySystemRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AAuraEnemy::DIE(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController) AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::DIE(DeathImpulse);
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("bHitReacting", false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool("bRangedAttacker", CharacterClass != ECharacterClass::Warrior);
}

void AAuraEnemy::HitReactTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f: BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitReacting"), bHitReacting);
	}
}

void AAuraEnemy::OnStunTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	Super::OnStunTagChanged(CallbackTag, NewCount);
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("bIsStunned"), bIsStunned);
	}
}

void AAuraEnemy::HighlightActor_Implementation()
{
	bHighlighted = true;
	
	if (GetMesh() == nullptr || Weapon == nullptr) return;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	bHighlighted = false;
	
	if (GetMesh() == nullptr || Weapon == nullptr) return;
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	
	Weapon->SetRenderCustomDepth(false);
	Weapon->SetCustomDepthStencilValue(0);
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	if (InCombatTarget == nullptr) return;
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	if (CombatTarget == nullptr) return nullptr;
	return CombatTarget;
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}


