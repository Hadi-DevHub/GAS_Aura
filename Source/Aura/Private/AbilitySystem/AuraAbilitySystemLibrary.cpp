// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameMode/AuraGameModeBase.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Engine/OverlapResult.h"
#include "Interaction/CombatInterface.h"


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

UAuraSpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutHUD)
{
	if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0)))
	{
		OutHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(PS->GetAbilitySystemComponent());
			UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(PS->GetAttributeSet());

			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharaClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharaClassInfo->GetClassDefaultInfo(CharacterClass);
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();

	AActor* AvatarActor = ASC->GetAvatarActor();
	EffectContextHandle.AddSourceObject(AvatarActor);

	FGameplayEffectSpecHandle EffectSpecHandlePrimary = ASC->MakeOutgoingSpec(CharacterClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandlePrimary.Data.Get());

	FGameplayEffectSpecHandle EffectSpecHandleSecondary = ASC->MakeOutgoingSpec(CharaClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandleSecondary.Data.Get());

	FGameplayEffectSpecHandle EffectSpecHandleVitals = ASC->MakeOutgoingSpec(CharaClassInfo->VitalAttributes, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandleVitals.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,UAbilitySystemComponent* ThisASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharaClassInfo = GetCharacterClassInfo(WorldContextObject);
	
	for (TSubclassOf<UGameplayAbility> Ability : CharaClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		ThisASC->GiveAbility(AbilitySpec);
	}

	FCharacterClassDefaultInfo DefaultInfo = CharaClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> Ability : DefaultInfo.StartupAbilities)
	{
		int32 AbilityLevel = 1;
		if (ThisASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			AbilityLevel = ICombatInterface::Execute_GetPlayerLevel(ThisASC->GetAvatarActor());
		}
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
		ThisASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	
	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	
	return AuraGameMode->AbilityInfo;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceASC->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags::Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags::Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags::Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraGameplayTags::Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}
									//------------------//
									//		SETTERS		//
									//------------------//

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& Context, bool bIsInBlock)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetIsBlockingHit(bIsInBlock);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& Context, bool bIsInCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bIsInCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& Context,
	bool bIsInSuccessfulDebuff)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetIsSuccessfulDebuff(bIsInSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& Context, float InDebuffDamage)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& Context, float InDebuffDuration)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& Context,
	float InDebuffFrequency)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& Context,
	const FGameplayTag& InDamageType)
{
	if ( FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		AuraEffectContext->SetDamageType(DamageType);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& Context, const FVector& InVector)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InVector);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& Context, const FVector& InVector)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(Context.Get()))
	{
		AuraEffectContext->SetKnockbackForce(InVector);
	}
}

	//------------------//
	//		GETTERS		//
	//------------------//							

bool UAuraAbilitySystemLibrary::GetIsBlockedHit(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetIsBlockingHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetIsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::GetIsSuccessfulDebuff(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetIsSuccessfulDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return *AuraEffectContext->GetDamageType();
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& Context)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(Context.Get()))
	{
		return AuraEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& ForwardDirection, const FVector& Axis, float SpreadAngle, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	FVector LeftOfSpread = ForwardDirection.RotateAngleAxis(-SpreadAngle / 2, Axis);

	if (NumRotators > 1)
	{
		const float DeltaRotation = SpreadAngle / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			FVector VectorDir = LeftOfSpread.RotateAngleAxis(DeltaRotation * i, FVector::UpVector);
			Rotators.Add(VectorDir.Rotation());
		}
		return Rotators;
	}
	Rotators.Add(ForwardDirection.Rotation());
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& ForwardDirection, const FVector& Axis, float SpreadAngle, int32 NumVectors)
{
	TArray<FVector> Directions;
	FVector LeftOfSpread = ForwardDirection.RotateAngleAxis(-SpreadAngle / 2, Axis);

	if (NumVectors > 1)
	{
		const float DeltaRotation = SpreadAngle / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			FVector VectorDir = LeftOfSpread.RotateAngleAxis(DeltaRotation * i, FVector::UpVector);
			Directions.Add(VectorDir);
		}
		return Directions;
	}
	Directions.Add(ForwardDirection);
	return Directions;
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects), FCollisionShape::MakeSphere(radius), SphereParams);
		for (FOverlapResult Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_GetIsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(Overlap.GetActor()));
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() < MaxTargets)
	{
		OutClosestTargets = Actors;
	}
	
	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor = nullptr;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestActor = PotentialTarget;
				ClosestDistance = Distance;
			}
		}
		OutClosestTargets.AddUnique(ClosestActor);
		ActorsToCheck.Remove(ClosestActor);
		NumTargetsFound++;
	}
	
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* ThisActor, AActor* OtherActor)
{
	bool bIsThisPlayer = ThisActor->ActorHasTag("Player");
	bool bIsOtherPlayer = OtherActor->ActorHasTag("Player");;

	bool bIsBothPlayer = bIsThisPlayer && bIsOtherPlayer;
	bool bIsBotNotPlayer = !bIsThisPlayer && !bIsOtherPlayer;

	bool bIsFriend = bIsBothPlayer || bIsBotNotPlayer;
	
	return !bIsFriend;
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 Level)
{
	const UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (ClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = ClassInfo->GetClassDefaultInfo(CharacterClass);
	float XPReward = Info.XPReward.GetValueAtLevel(Level);
	return static_cast<int32>(XPReward);
}
