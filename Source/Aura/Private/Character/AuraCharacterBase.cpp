// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Material/DynamicMaterialInstance.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetIsReplicated(true);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

AActor* AAuraCharacterBase::GetAvatarActor_Implementation() 
{
	return this;
}

bool AAuraCharacterBase::GetIsDead() const
{
	return bIsDead;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AuraASC->AddCharacterAbilities(StartupAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(CharacterDissolveMaterial))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(CharacterDissolveMaterial, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartCharacterDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterial))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterial, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

TArray<FTaggedMontages> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return TaggedMontages;
}

void AAuraCharacterBase::DIE()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float EffectLevel) const
{
	check(IsValid(AbilitySystemComponent));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, EffectLevel, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	check(HitReactAnim)
	return HitReactAnim;
}



