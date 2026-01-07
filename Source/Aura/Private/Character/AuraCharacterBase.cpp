// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/AuraCharacterBase.h"

#include "AuraGameplayTags.h"
#include "AbilityStatusNiagara/AuraPassiveNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "AbilityStatusNiagara//AuraDebuffNiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	BurnDebuffComponent = CreateDefaultSubobject<UAuraDebuffNiagaraComponent>("BurnDebuffNiagaraComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = AuraGameplayTags::Debuff_Burn;
	
	StunDebuffComponent = CreateDefaultSubobject<UAuraDebuffNiagaraComponent>("StunDebuffNiagaraComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = AuraGameplayTags::Debuff_Stun;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Weapon->SetIsReplicated(true);

	//-------------------//
	//    Passive VFX    //
	//-------------------//
	
	EffectAttachmentLocation = CreateDefaultSubobject<USceneComponent>("EffectAttachmentLocation");
	EffectAttachmentLocation->SetupAttachment(GetRootComponent());
	
	HaloOfProtectionComponent = CreateDefaultSubobject<UAuraPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionComponent->SetupAttachment(EffectAttachmentLocation);
	LifeSiphonComponent = CreateDefaultSubobject<UAuraPassiveNiagaraComponent>("LifeSiphonComponent");
	LifeSiphonComponent->SetupAttachment(EffectAttachmentLocation);
	ManaSiphonComponent = CreateDefaultSubobject<UAuraPassiveNiagaraComponent>("ManaSiphonComponent");
	ManaSiphonComponent->SetupAttachment(EffectAttachmentLocation);

	//-------------------//
	//    Passive VFX    //
	//-------------------//
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	if (MontageTag.MatchesTagExact(AuraGameplayTags::CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag == AuraGameplayTags::CombatSocket_RightHand && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag == AuraGameplayTags::CombatSocket_LeftHand && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag == AuraGameplayTags::CombatSocket_Tail && IsValid(GetMesh()))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
}

AActor* AAuraCharacterBase::GetAvatarActor_Implementation() 
{
	return this;
}

bool AAuraCharacterBase::GetIsDead_Implementation()
{
	return bIsDead;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

int32 AAuraCharacterBase::GetNumberOfMinions_Implementation() const
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementNumberOfMinions_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EffectAttachmentLocation->SetWorldRotation(FRotator(FRotator::ZeroRotator));
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

void AAuraCharacterBase::AddCharacterPassiveAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

void AAuraCharacterBase::OnStunTagChanged(FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f: BaseWalkSpeed;
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

void AAuraCharacterBase::OnRep_IsStunned() const
{

}

void AAuraCharacterBase::OnRep_IsBurned() const
{
	
}

TArray<FTaggedMontages> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return TaggedMontages;
}

FTaggedMontages AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) 
{
	for (auto TaggedMontage : TaggedMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontages();
}

void AAuraCharacterBase::DIE(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	}
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse, NAME_None, true);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	bIsDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (BurnDebuffComponent) BurnDebuffComponent->Deactivate();
	if (StunDebuffComponent) StunDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
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

FOnAbilitySystemRegistered& AAuraCharacterBase::DelegateToOnAbilitySystemRegistered() 
{
	return AbilitySystemRegisteredDelegate;
}

FOnDeathSignature& AAuraCharacterBase::GetDelegateToOnDeath() 
{
	return OnDeathDelegate;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	check(HitReactAnim)
	return HitReactAnim;
}



