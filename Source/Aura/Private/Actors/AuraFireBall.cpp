// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValidHit(OtherActor))
	{
		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				const FVector ForwardVector = GetActorForwardVector();
				const FVector DeathImpulse =  ForwardVector * DamageEffectParams.DeathImpulseMagnitude;
			
				DamageEffectParams.DeathImpulse = DeathImpulse;
				DamageEffectParams.TargetASC = TargetASC;
				UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}
		}
	}
}

void AAuraFireBall::OnHit() const
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), AuraGameplayTags::GameplayCue_FireBlast, CueParams);
	}
		
	if (LoopingSFXComponent)
	{
		LoopingSFXComponent->Stop();
		LoopingSFXComponent->DestroyComponent();
	}
	if (LoopingSFXComponent) LoopingSFXComponent->Stop();
}
