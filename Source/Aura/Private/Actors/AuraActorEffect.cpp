#include "Actors/AuraActorEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

AAuraActorEffect::AAuraActorEffect()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
}

void AAuraActorEffect::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();
}

void AAuraActorEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float SinePeriod = 2 * PI * SinePeriodConstant;
	RunningTime += DeltaTime;
	if (RunningTime > SinePeriod)
	{
		RunningTime = 0;
	}
	ItemMovement(DeltaTime);
}

void AAuraActorEffect::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AAuraActorEffect::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}

void AAuraActorEffect::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<class UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag("Enemy") && !bbApplyEffectToEnemies) return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite)
	{
		ActiveGameplayEffects.Add(ActiveEffectHandle, TargetASC);
	}

	if (!bIsInfinite)
	{
		Destroy();
	}
}

void AAuraActorEffect::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
}

void AAuraActorEffect::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag("Enemy") && !bbApplyEffectToEnemies) return;
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (auto HandlePair : ActiveGameplayEffects)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveGameplayEffects.FindAndRemoveChecked(Handle);
		}
	}
}

void AAuraActorEffect::ItemMovement(float DeltaTime)
{
	if (bRotates)
 	{
 		const FRotator DeltaRotation(0, DeltaTime * RotationRate, 0);
 		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
 	}
	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
		CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
	}
	
}
